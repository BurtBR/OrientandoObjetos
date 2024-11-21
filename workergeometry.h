#ifndef WORKERGEOMETRY_H
#define WORKERGEOMETRY_H

#include <QObject>
#include <QHash>
#include "errormessage.h"
#include "vertice.h"
#include "edge.h"
#include "face.h"
#include "operation.h"

class WorkerGeometry : public QObject{
    Q_OBJECT
private:
    QHash<size_t, Vertice> _vertices;
    QHash<size_t, Edge> _edges;
    QHash<size_t, Face> _faces;
    QVector<Operation> _ops;
    QMatrix4x4 _opMatrix;

    bool StrToFloat(const QString &str, float &number);
    void ParseFileData();
    QHash<size_t, Edge>::Iterator FindEquivalentEdge(size_t edge);
    void CheckDuplicateVertices();
    void CheckVerticesIncidentEdges();
    void ReplaceVerticeReference(size_t from, size_t to);
    void ReplaceEdgeReferences(size_t from, size_t to);
    void SetFaceToSide(size_t face, size_t startedge, bool left = true);
    void FlipFaceEdges(size_t face);
    void FlipFaceEdgesHorizontal(size_t face);
    void FlipFaceEdgesVertical(size_t face);
    void FlipEdge(QHash<size_t, Edge>::Iterator &e);
    void FlipEdgeVerticalAxis(QHash<size_t, Edge>::Iterator &e);
    void FlipEdgeHorizontalAxis(QHash<size_t, Edge>::Iterator &e);
    void SendVerticeList();
    void SendEdgeList();
    void SendFaceList();
    void SendOperations();
    void CalculateOpMatrix();

public:
    WorkerGeometry(QObject *parent = nullptr);
    ~WorkerGeometry();

public slots:
    void GetSelectedVertice(size_t id);
    void GetSelectedEdge(size_t id);
    void GetSelectedFace(size_t id);
    void OpenObj(QString filename);
    void PrintAllData();
    void PrintVerticesFromFace(size_t f);
    void PrintFacesFromEdge(size_t e);
    void PrintEdgesFromVertice(size_t v);
    void AddOperation(float x, float y, float z, Operation::OpType op);
    void RemoveOperation(size_t idx);

signals:
    void SetVerticeList(QStringList list);
    void SetEdgeList(QStringList list);
    void SetFaceList(QStringList list);
    void SetSelectedVerticeData(Vertice v);
    void SetSelectedEdgeData(Edge e);
    void SetSelectedFaceData(Face f);
    void Message(QString, ErrorMessage::ErrorCode code = ErrorMessage::ErrorCode::Misc);
    void FileHandlingFinished();
    void SetOperationMatrix(QMatrix4x4 M);
    void SetOperationList(QStringList list);
};

#endif // WORKERGEOMETRY_H
