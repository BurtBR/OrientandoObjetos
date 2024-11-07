#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include "workerfilehandler.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _ui(new Ui::MainWindow){
    _ui->setupUi(this);
}

MainWindow::~MainWindow(){
    DeleteThread(&_threadFileHandler);
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
    return true;
}

void MainWindow::WorkerMessage(QString msg){

}

void MainWindow::FileHandlingFinished(){

}

void MainWindow::ConsoleMessage(QString msg){
    _ui->textConsole->append(QDateTime::currentDateTime().toString("hh:mm:ss") + " " + msg);
}

bool MainWindow::StartThreadFileHandler(){
    if(_threadFileHandler)
        return false;

    WorkerFileHandler *worker;

    try{
        worker = new WorkerFileHandler;
    }catch(...){
        return false;
    }

    try{
        _threadFileHandler = new QThread;
    }catch(...){
        delete worker;
        _threadFileHandler = nullptr;
        return false;
    }

    connect(_threadFileHandler, &QThread::finished, worker, &WorkerFileHandler::deleteLater);

    return true;
}

void MainWindow::On_actionAbrir_triggered(bool){
    QString filename = QFileDialog::getOpenFileName(this, "Abrir arquivo", "", "*.obj");

    if(!filename.size())
        return;


}
