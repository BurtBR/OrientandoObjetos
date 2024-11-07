#ifndef WORKERFILEHANDLER_H
#define WORKERFILEHANDLER_H

#include <QObject>

class WorkerFileHandler : public QObject{
    Q_OBJECT
public:
    explicit WorkerFileHandler(QObject *parent = nullptr);

public slots:
    void OpenFile(QString filename);

signals:
    void Message(QString);
};

#endif // WORKERFILEHANDLER_H
