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
    uint64_t StrToAddr(QString str);
    void ParseFileData();
    Edge *FindEdge(Vertice *v1, Vertice *v2);
    void SendVerticeList();
    void SendEdgeList();
    void SendFaceList();
    void SendOperations();
    void CalculateOpMatrix();

public:
    WorkerGeometry(QObject *parent = nullptr);
    ~WorkerGeometry();

public slots:
    void GetSelectedVertice(QString str);
    void GetSelectedEdge(QString str);
    void GetSelectedFace(QString str);
    void OpenObj(QString filename);
    void PrintAllData();
    void PrintVerticesFromFace(QString str);
    void PrintFacesFromEdge(QString str);
    void PrintEdgesFromVertice(QString str);
    void AddOperation(float x, float y, float z, Operation::OpType op);
    void RemoveOperation(size_t idx);
    void MoveOperationUp(size_t idx);
    void MoveOperationDown(size_t idx);
    void SetOperationXYZ(size_t idx, float x, float y, float z);
    void GetSelectedOperation(size_t idx);

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
    void SetSelectedOperation(float x, float y, float z);
};

#endif // WORKERGEOMETRY_H
