#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMovie>
#include "workergeometry.h"

#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _ui(new Ui::MainWindow){
    _ui->setupUi(this);
    _ui->stackedWidgetOptions->setCurrentWidget(_ui->pageMT);
}

MainWindow::~MainWindow(){
    DeleteThread(&_threadGeometry);
    delete _ui;
}

void MainWindow::DeleteThread(QThread **thread){
    if(!thread)
        return;

    QThread *t = *thread;
    if(t){
        t->quit();
        if(!t->wait(5000)){
            t->terminate();
            t->wait();
        }
        delete *thread;
        *thread = nullptr;
    }
}

bool MainWindow::Init(){
    connect(_ui->actionAbrir, &QAction::triggered, this, &MainWindow::On_actionAbrir_triggered);
    connect(_ui->buttonClearConsole, &QToolButton::clicked, this, &MainWindow::On_buttonClearConsole_Clicked);
    connect(_ui->buttonVertices, &QToolButton::clicked, this, &MainWindow::On_buttonVertices_Clicked);
    connect(_ui->buttonEdges, &QToolButton::clicked, this, &MainWindow::On_buttonEdges_Clicked);
    connect(_ui->buttonFaces, &QToolButton::clicked, this, &MainWindow::On_buttonFaces_Clicked);
    connect(_ui->listVertices, &QListWidget::currentRowChanged, this, &MainWindow::On_listVertices_SelectionChanged);
    connect(_ui->listEdges, &QListWidget::currentRowChanged, this, &MainWindow::On_listEdges_SelectionChanged);
    connect(_ui->listFaces, &QListWidget::currentRowChanged, this, &MainWindow::On_listFaces_SelectionChanged);

    if(!StartThreadGeometry())
        return false;

    return true;
}

void MainWindow::WorkerMessage(QString msg, ErrorMessage::ErrorCode code){
    ConsoleMessage(ErrorMessage::GetErrorStr(code) + msg);
}

void MainWindow::FileHandlingFinished(){

    ConsoleMessage("Manipulação de arquivo concluída.");

    this->setEnabled(true);
    _ui->labelGif->clear();
}

void MainWindow::SetVerticeList(QStringList list){
    _ui->listVertices->clear();
    _ui->listVertices->addItems(list);
    _ui->lineXVer->clear();
    _ui->lineYVer->clear();
    _ui->lineZVer->clear();
    _ui->lineEdVer->clear();
}

void MainWindow::SetEdgeList(QStringList list){
    _ui->listEdges->clear();
    _ui->listEdges->addItems(list);
    _ui->lineVOri->clear();
    _ui->lineVDest->clear();
    _ui->lineFDir->clear();
    _ui->lineFEsq->clear();
    _ui->lineDirIn->clear();
    _ui->lineDirOut->clear();
    _ui->lineEsqIn->clear();
    _ui->lineEsqOut->clear();
}

void MainWindow::SetFaceList(QStringList list){
    _ui->listFaces->clear();
    _ui->listFaces->addItems(list);
    _ui->lineEdgOfFace->clear();
}

void MainWindow::SetSelectedVerticeData(Vertice v){
    _ui->lineXVer->setText(QString::number(v.GetX()));
    _ui->lineYVer->setText(QString::number(v.GetY()));
    _ui->lineZVer->setText(QString::number(v.GetZ()));
    _ui->lineEdVer->setText(QString::number((int)v.GetIncidentEdge()));
}

void MainWindow::SetSelectedEdgeData(Edge e){
    _ui->lineVOri->setText(QString::number((int)e.GetVerticeOrigin()));
    _ui->lineVDest->setText(QString::number((int)e.GetVerticeDestination()));
    _ui->lineFDir->setText(QString::number((int)e.GetFaceRight()));
    _ui->lineFEsq->setText(QString::number((int)e.GetFaceLeft()));
    _ui->lineDirIn->setText(QString::number((int)e.GetEdgeRightIn()));
    _ui->lineDirOut->setText(QString::number((int)e.GetEdgeRightOut()));
    _ui->lineEsqIn->setText(QString::number((int)e.GetEdgeLeftIn()));
    _ui->lineEsqOut->setText(QString::number((int)e.GetEdgeLeftOut()));
}

void MainWindow::SetSelectedFaceData(Face f){
    _ui->lineEdgOfFace->setText(QString::number((int)f.GetEdge()));
}

void MainWindow::ConsoleMessage(QString msg){
    _ui->textConsole->append(QDateTime::currentDateTime().toString("hh:mm:ss") + " " + msg);
}

bool MainWindow::StartThreadGeometry(){
    if(_threadGeometry)
        return false;

    WorkerGeometry *worker;

    try{
        worker = new WorkerGeometry;
    }catch(...){
        return false;
    }

    try{
        _threadGeometry = new QThread;
    }catch(...){
        delete worker;
        return false;
    }

    connect(_threadGeometry, &QThread::finished, worker, &WorkerGeometry::deleteLater);
    connect(worker, &WorkerGeometry::SetVerticeList, this, &MainWindow::SetVerticeList);
    connect(worker, &WorkerGeometry::SetEdgeList, this, &MainWindow::SetEdgeList);
    connect(worker, &WorkerGeometry::SetFaceList, this, &MainWindow::SetFaceList);
    connect(worker, &WorkerGeometry::SetSelectedVerticeData, this, &MainWindow::SetSelectedVerticeData);
    connect(worker, &WorkerGeometry::SetSelectedEdgeData, this, &MainWindow::SetSelectedEdgeData);
    connect(worker, &WorkerGeometry::SetSelectedFaceData, this, &MainWindow::SetSelectedFaceData);
    connect(worker, &WorkerGeometry::Message, this, &MainWindow::WorkerMessage);
    connect(worker, &WorkerGeometry::FileHandlingFinished, this, &MainWindow::FileHandlingFinished);

    connect(this, &MainWindow::OpenObj, worker, &WorkerGeometry::OpenObj);
    connect(this, &MainWindow::GetSelectedVertice, worker, &WorkerGeometry::GetSelectedVertice);
    connect(this, &MainWindow::GetSelectedEdge, worker, &WorkerGeometry::GetSelectedEdge);
    connect(this, &MainWindow::GetSelectedFace, worker, &WorkerGeometry::GetSelectedFace);

    connect(_ui->buttonPrintStructures, &QToolButton::clicked, worker, &WorkerGeometry::PrintAllData);

    worker->moveToThread(_threadGeometry);
    _threadGeometry->start();

    return true;
}

void MainWindow::On_listVertices_SelectionChanged(int idx){
    emit GetSelectedVertice(_ui->listVertices->item(idx)->text().split(' ', Qt::SkipEmptyParts)[1].toULongLong());
}

void MainWindow::On_listEdges_SelectionChanged(int idx){
    emit GetSelectedEdge(_ui->listEdges->item(idx)->text().split(' ', Qt::SkipEmptyParts)[1].toULongLong());
}

void MainWindow::On_listFaces_SelectionChanged(int idx){
    emit GetSelectedFace(_ui->listFaces->item(idx)->text().split(' ', Qt::SkipEmptyParts)[1].toULongLong());
}

void MainWindow::On_actionAbrir_triggered(bool){
    QString filename = QFileDialog::getOpenFileName(this, "Abrir arquivo", "", "*.obj");

    if(!filename.size())
        return;

    this->setEnabled(false);
    QMovie *movie;

    try{
        movie = new QMovie(":/Images/Loading.gif");
    }catch(...){
        emit OpenObj(filename);
        return;
    }

    _ui->labelGif->setMovie(movie);
    _ui->labelGif->setScaledContents(true);
    movie->start();

    emit OpenObj(filename);
}

void MainWindow::On_buttonVertices_Clicked(){
    _ui->stackedWidgetOptions->setCurrentWidget(_ui->pageVertices);
}

void MainWindow::On_buttonEdges_Clicked(){
    _ui->stackedWidgetOptions->setCurrentWidget(_ui->pageEdges);
}

void MainWindow::On_buttonFaces_Clicked(){
    _ui->stackedWidgetOptions->setCurrentWidget(_ui->pageFaces);
}

void MainWindow::On_buttonClearConsole_Clicked(){
    _ui->textConsole->clear();
}
