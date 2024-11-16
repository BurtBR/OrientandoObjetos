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
    connect(worker, &WorkerGeometry::Message, this, &MainWindow::WorkerMessage);
    connect(worker, &WorkerGeometry::FileHandlingFinished, this, &MainWindow::FileHandlingFinished);

    connect(this, &MainWindow::OpenObj, worker, &WorkerGeometry::OpenObj);

    connect(_ui->buttonPrintStructures, &QToolButton::clicked, worker, &WorkerGeometry::PrintAllData);

    worker->moveToThread(_threadGeometry);
    _threadGeometry->start();

    return true;
}

void MainWindow::On_listVertices_SelectionChanged(int idx){
    ConsoleMessage(_ui->listVertices->item(idx)->text());
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
