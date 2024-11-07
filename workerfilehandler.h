#ifndef WORKERFILEHANDLER_H
#define WORKERFILEHANDLER_H

#include <QObject>
#include "errormessage.h"

class WorkerFileHandler : public QObject{
    Q_OBJECT
public:
    explicit WorkerFileHandler(QObject *parent = nullptr);

public slots:
    void OpenObj(QString filename);

signals:
    void Message(QString, ErrorMessage::ErrorCode code = ErrorMessage::ErrorCode::Misc);
    void FileHandlingFinished();
};

#endif // WORKERFILEHANDLER_H
