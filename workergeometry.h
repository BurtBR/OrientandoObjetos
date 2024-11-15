#ifndef WORKERGEOMETRY_H
#define WORKERGEOMETRY_H

#include <QObject>
#include <QHash>
#include "errormessage.h"
#include "vertice.h"
#include "edge.h"
#include "face.h"

class WorkerGeometry : public QObject{
    Q_OBJECT
private:
    QHash<size_t, Vertice> _vertices;
    QHash<size_t, Edge> _edges;
    QHash<size_t, Face> _faces;

    bool StrToFloat(const QString &str, float &number);
    void ParseFileData();
    QHash<size_t, Edge>::Iterator FindEquivalentEdge(size_t edge);
    void CheckDuplicateVertices();
    void ReplaceVerticeReference(size_t from, size_t to);
    void ReplaceEdgeReferences(size_t from, size_t to);
    void SetFaceToSide(size_t face, bool left = true);
    void FlipEdge(QHash<size_t, Edge>::Iterator &e);

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
