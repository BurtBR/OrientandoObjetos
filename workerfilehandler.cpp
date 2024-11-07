#include "workerfilehandler.h"
#include <QFile>
#include <QFileInfo>

// TEMP
#include <QThread>

WorkerFileHandler::WorkerFileHandler(QObject *parent) : QObject{parent}{

}

void WorkerFileHandler::OpenObj(QString filename){
    QFile fp(filename);
    QFileInfo fileinfo(fp);

    if(!fileinfo.exists()){
        emit Message("O arquivo não existe", ErrorMessage::ErrorCode::FailedToOpenFile);
        emit FileHandlingFinished();
        return;
    }

    emit Message("Carregando arquivo: " + fileinfo.fileName());

    QThread::sleep(5);

    emit FileHandlingFinished();
}
