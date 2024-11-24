#ifndef WORKERGEOMETRY_H
#define WORKERGEOMETRY_H

#include <QObject>
#include <list>
#include "errormessage.h"
#include "vertice.h"
#include "edge.h"
#include "face.h"
#include "operation.h"

class WorkerGeometry : public QObject{
    Q_OBJECT
public:
    struct VerticeData{
        const Vertice *_addr;
        QString _id;
        QString _x, _y, _z;
        QString _incident;
    };

    struct EdgeData{
        const Edge *_addr;
        QString _id;
        QString _verticeUp, _verticeDown;
        QString _faceLeft, _faceRight;
        QString _edgeLeftUp, _edgeLeftDown;
        QString _edgeRightUp, _edgeRightDown;
    };

    struct FaceData{
        const Face *_addr;
        QString _id;
        QString _edge;
    };

private:

    std::list<Vertice> _vertices;
    std::list<Edge> _edges;
    std::list<Face> _faces;
    QVector<Operation> _ops;
    QMatrix4x4 _opMatrix;

    bool InsertVertice(QStringList list, QVector<Vertice*> &vectorvertices);
    bool InsertFace(QStringList list, QVector<Vertice*> &vectorvertices);
    void ClearGeometry();
    void FailedToOpenObj();
    bool StrToFloat(const QString &str, float &number);
    Edge *FindEdge(Vertice *v1, Vertice *v2);
    void SendVerticeList();
    void SendEdgeList();
    void SendFaceList();
    void SendOperations();
    void CalculateOpMatrix();

public:
    WorkerGeometry(QObject *parent = nullptr);
    ~WorkerGeometry();

    static VerticeData ParseVertice(const Vertice *v);
    static EdgeData ParseEdge(const Edge *e);
    static FaceData ParseFace(const Face *f);

public slots:
    void GetSelectedVertice(const Vertice *v);
    void GetSelectedEdge(const Edge *e);
    void GetSelectedFace(const Face *f);
    void OpenObj(QString filename);
    void PrintAllData();
    void PrintVerticesFromFace(const Face *f);
    void PrintFacesFromEdge(const Edge *e);
    void PrintEdgesFromVertice(const Vertice *v);
    void AddOperation(float x, float y, float z, Operation::OpType op);
    void RemoveOperation(size_t idx);
    void MoveOperationUp(size_t idx);
    void MoveOperationDown(size_t idx);
    void SetOperationXYZ(size_t idx, float x, float y, float z);
    void GetSelectedOperation(size_t idx);

signals:
    void SetVerticeList(const QVector<const Vertice*> verticelist, QStringList strlist);
    void SetEdgeList(const QVector<const Edge*> edgelist, QStringList strlist);
    void SetFaceList(const QVector<const Face*> facelist, QStringList strlist);
    void SetSelectedVerticeData(VerticeData v);
    void SetSelectedEdgeData(EdgeData e);
    void SetSelectedFaceData(FaceData f);
    void Message(QString, ErrorMessage::ErrorCode code = ErrorMessage::ErrorCode::Misc);
    void FileHandlingFinished();
    void SetOperationMatrix(QMatrix4x4 M);
    void SetOperationList(QStringList list);
    void SetSelectedOperation(float x, float y, float z);
};

#endif // WORKERGEOMETRY_H
