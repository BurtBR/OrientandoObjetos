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
    connect(_ui->buttonOperations, &QToolButton::clicked, this, &MainWindow::On_buttonOperations_Clicked);
    connect(_ui->buttonOpMatrix, &QToolButton::clicked, this, &MainWindow::On_buttonOpMatrix_Clicked);
    connect(_ui->buttonAddOp, &QToolButton::clicked, this, &MainWindow::On_buttonAddOp_Clicked);
    connect(_ui->buttonRemoveOp, &QToolButton::clicked, this, &MainWindow::On_buttonRemoveOp_Clicked);
    connect(_ui->buttonOpBack, &QToolButton::clicked, this, &MainWindow::On_buttonOpBack_Clicked);
    connect(_ui->buttonOpUp, &QToolButton::clicked, this, &MainWindow::On_buttonOpUp_Clicked);
    connect(_ui->buttonOpDown, &QToolButton::clicked, this, &MainWindow::On_buttonOpDown_Clicked);
    connect(_ui->buttonSetOpParam, &QToolButton::clicked, this, &MainWindow::On_buttonSetOpParam_Clicked);
    connect(_ui->buttonFacesFromEdges, &QToolButton::clicked, this, &MainWindow::On_buttonFacesFromEdge_Clicked);
    connect(_ui->buttonEdgesFromVertices, &QToolButton::clicked, this, &MainWindow::On_buttonEdgesFromVertices_Clicked);
    connect(_ui->buttonVerticesFromFace, &QToolButton::clicked, this, &MainWindow::On_buttonVerticesFromFace_Clicked);
    connect(_ui->listVertices, &QListWidget::currentRowChanged, this, &MainWindow::On_listVertices_SelectionChanged);
    connect(_ui->listEdges, &QListWidget::currentRowChanged, this, &MainWindow::On_listEdges_SelectionChanged);
    connect(_ui->listFaces, &QListWidget::currentRowChanged, this, &MainWindow::On_listFaces_SelectionChanged);
    connect(_ui->listOperations, &QListWidget::currentRowChanged, this, &MainWindow::On_listOperations_SelectionChanged);

    if(!StartThreadGeometry())
        return false;

    try{
        _ui->lineOpX->setValidator(new QDoubleValidator);
        _ui->lineOpY->setValidator(new QDoubleValidator);
        _ui->lineOpZ->setValidator(new QDoubleValidator);
    }catch(...){
        return false;
    }

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

void MainWindow::SetOperationMatrix(QMatrix4x4 M){
    float *data = M.data();

    _ui->label_M11->setText(QString::number(data[0], 'f', 1));
    _ui->label_M12->setText(QString::number(data[4], 'f', 1));
    _ui->label_M13->setText(QString::number(data[8], 'f', 1));
    _ui->label_M14->setText(QString::number(data[12], 'f', 1));
    _ui->label_M21->setText(QString::number(data[1], 'f', 1));
    _ui->label_M22->setText(QString::number(data[5], 'f', 1));
    _ui->label_M23->setText(QString::number(data[9], 'f', 1));
    _ui->label_M24->setText(QString::number(data[13], 'f', 1));
    _ui->label_M31->setText(QString::number(data[2], 'f', 1));
    _ui->label_M32->setText(QString::number(data[6], 'f', 1));
    _ui->label_M33->setText(QString::number(data[10], 'f', 1));
    _ui->label_M34->setText(QString::number(data[14], 'f', 1));
    _ui->label_M41->setText(QString::number(data[3], 'f', 1));
    _ui->label_M42->setText(QString::number(data[7], 'f', 1));
    _ui->label_M43->setText(QString::number(data[11], 'f', 1));
    _ui->label_M44->setText(QString::number(data[15], 'f', 1));
}

void MainWindow::SetOperationList(QStringList list){

    int selected = _ui->listOperations->currentRow();

    if(selected != -1){
        if((_ui->listOperations->count() > list.size()) && selected > 0){
            selected--;
        }else if(_ui->listOperations->count() < list.size()){
            selected = list.size()-1;
        }
        _ui->listOperations->clear();
        _ui->listOperations->addItems(list);
        _ui->listOperations->setCurrentRow(selected);
    }else{
        _ui->listOperations->clear();
        _ui->listOperations->addItems(list);
    }
}

void MainWindow::SetSelectedOperation(float x, float y, float z){
    _ui->lineOpX->setText(QString::number(x));
    _ui->lineOpY->setText(QString::number(y));
    _ui->lineOpZ->setText(QString::number(z));
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
    connect(worker, &WorkerGeometry::SetOperationList, this, &MainWindow::SetOperationList);
    connect(worker, &WorkerGeometry::SetOperationMatrix, this, &MainWindow::SetOperationMatrix);
    connect(worker, &WorkerGeometry::SetSelectedOperation, this, &MainWindow::SetSelectedOperation);

    connect(this, &MainWindow::OpenObj, worker, &WorkerGeometry::OpenObj);
    connect(this, &MainWindow::GetSelectedVertice, worker, &WorkerGeometry::GetSelectedVertice);
    connect(this, &MainWindow::GetSelectedEdge, worker, &WorkerGeometry::GetSelectedEdge);
    connect(this, &MainWindow::GetSelectedFace, worker, &WorkerGeometry::GetSelectedFace);
    connect(this, &MainWindow::PrintFacesFromEdge, worker, &WorkerGeometry::PrintFacesFromEdge);
    connect(this, &MainWindow::PrintVerticesFromFace, worker, &WorkerGeometry::PrintVerticesFromFace);
    connect(this, &MainWindow::PrintEdgesFromVertice, worker, &WorkerGeometry::PrintEdgesFromVertice);
    connect(this, &MainWindow::AddOperation, worker, &WorkerGeometry::AddOperation);
    connect(this, &MainWindow::RemoveOperation, worker, &WorkerGeometry::RemoveOperation);
    connect(this, &MainWindow::MoveOperationUp, worker, &WorkerGeometry::MoveOperationUp);
    connect(this, &MainWindow::MoveOperationDown, worker, &WorkerGeometry::MoveOperationDown);
    connect(this, &MainWindow::SetOperationXYZ, worker, &WorkerGeometry::SetOperationXYZ);
    connect(this, &MainWindow::GetSelectedOperation, worker, &WorkerGeometry::GetSelectedOperation);

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

void MainWindow::On_listOperations_SelectionChanged(int idx){
    if(idx < 0)
        return;
    emit GetSelectedOperation(idx);
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

void MainWindow::On_buttonOperations_Clicked(){
    _ui->stackedWidgetOptions->setCurrentWidget(_ui->pageOperation);
}

void MainWindow::On_buttonOpMatrix_Clicked(){
    _ui->stackedWidgetOptions->setCurrentWidget(_ui->pageOpMatrix);
}

void MainWindow::On_buttonAddOp_Clicked(){

    float x, y, z;

    if(!_ui->lineOpX->text().size())
        x = 0;
    else
        x = _ui->lineOpX->text().toFloat();

    if(!_ui->lineOpY->text().size())
        y = 0;
    else
        y = _ui->lineOpY->text().toFloat();

    if(!_ui->lineOpZ->text().size())
        z = 0;
    else
        z = _ui->lineOpZ->text().toFloat();

    emit AddOperation(x, y, z, (Operation::OpType)_ui->comboOperations->currentIndex());
}

void MainWindow::On_buttonRemoveOp_Clicked(){
    if(_ui->listOperations->currentRow() == -1)
        return;

    emit RemoveOperation(_ui->listOperations->selectionModel()->selectedIndexes()[0].row());
}

void MainWindow::On_buttonOpUp_Clicked(){
    int selected = _ui->listOperations->currentRow();
    if((selected == -1) || (selected == 0))
        return;

    _ui->listOperations->setCurrentRow(selected-1);
    emit MoveOperationUp(selected);
}

void MainWindow::On_buttonOpDown_Clicked(){
    int selected = _ui->listOperations->currentRow();
    if((selected == -1) || (selected == (_ui->listOperations->count()-1)))
        return;

    _ui->listOperations->setCurrentRow(selected+1);
    emit MoveOperationDown(selected);
}

void MainWindow::On_buttonOpBack_Clicked(){
    _ui->stackedWidgetOptions->setCurrentWidget(_ui->pageOperation);
}

void MainWindow::On_buttonSetOpParam_Clicked(){
    int selected = _ui->listOperations->currentRow();
    float x, y, z;

    if(selected == -1)
        return;

    if(!_ui->lineOpX->text().size())
        x = 0;
    else
        x = _ui->lineOpX->text().toFloat();

    if(!_ui->lineOpY->text().size())
        y = 0;
    else
        y = _ui->lineOpY->text().toFloat();

    if(!_ui->lineOpZ->text().size())
        z = 0;
    else
        z = _ui->lineOpZ->text().toFloat();

    emit SetOperationXYZ(selected, x, y, z);
}

void MainWindow::On_buttonClearConsole_Clicked(){
    _ui->textConsole->clear();
}

void MainWindow::On_buttonVerticesFromFace_Clicked(){
    if(_ui->listFaces->selectedItems().size())
        emit PrintVerticesFromFace(_ui->listFaces->selectedItems()[0]->text().split(' ', Qt::SkipEmptyParts)[1].toULongLong());
}

void MainWindow::On_buttonFacesFromEdge_Clicked(){
    if(_ui->listEdges->selectedItems().size())
        emit PrintFacesFromEdge(_ui->listEdges->selectedItems()[0]->text().split(' ', Qt::SkipEmptyParts)[1].toULongLong());
}

void MainWindow::On_buttonEdgesFromVertices_Clicked(){
    if(_ui->listVertices->selectedItems().size())
        emit PrintEdgesFromVertice(_ui->listVertices->selectedItems()[0]->text().split(' ', Qt::SkipEmptyParts)[1].toULongLong());
}
