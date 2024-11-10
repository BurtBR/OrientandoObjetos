#ifndef WORKERGEOMETRY_H
#define WORKERGEOMETRY_H

#include <QObject>
#include <QSet>
#include "errormessage.h"
#include "vertice.h"
#include "edge.h"
#include "face.h"

class WorkerGeometry : public QObject{
    Q_OBJECT
private:
    QSet<Vertice> _vertices;
    QSet<Edge> _edges;
    QSet<Face> _faces;

    bool StrToFloat(const QString &str, float &number);

public:
    WorkerGeometry(QObject *parent = nullptr);
    ~WorkerGeometry();

public slots:
    void OpenObj(QString filename);

signals:
    void Message(QString, ErrorMessage::ErrorCode code = ErrorMessage::ErrorCode::Misc);
    void FileHandlingFinished();
};

#endif // WORKERGEOMETRY_H
