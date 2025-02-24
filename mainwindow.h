#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QThread>
#include "workergeometry.h"
#include "errormessage.h"
#include "vertice.h"
#include "edge.h"
#include "face.h"
#include "operation.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    Ui::MainWindow *_ui;
    QThread *_threadGeometry = nullptr;
    QVector<const Vertice*> _vertices;
    QVector<const Edge*> _edges;
    QVector<const Face*> _faces;

    void DeleteThread(QThread **thread);
    void ConsoleMessage(QString msg);
    bool StartThreadGeometry();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool Init();

private slots:
    void WorkerMessage(QString msg, ErrorMessage::ErrorCode code = ErrorMessage::ErrorCode::Misc);
    void FileHandlingFinished();
    void SetVerticeList(const QVector<const Vertice*> verticelist, QStringList strlist);
    void SetEdgeList(const QVector<const Edge*> edgelist, QStringList strlist);
    void SetFaceList(const QVector<const Face*> facelist, QStringList strlist);
    void SetSelectedVerticeData(WorkerGeometry::VerticeData v);
    void SetSelectedEdgeData(WorkerGeometry::EdgeData e);
    void SetSelectedFaceData(WorkerGeometry::FaceData f);
    void SetOperationMatrix(QMatrix4x4 M);
    void SetOperationList(QStringList list);
    void SetSelectedOperation(float x, float y, float z);
    void SetOpenGLVertexData(QVector<float> vertex);
    void OpenGLWindowSizeChanged();

    void On_listVertices_SelectionChanged(int idx);
    void On_listEdges_SelectionChanged(int idx);
    void On_listFaces_SelectionChanged(int idx);
    void On_listOperations_SelectionChanged(int idx);
    void On_actionAbrir_triggered(bool);
    void On_buttonVertices_Clicked();
    void On_buttonEdges_Clicked();
    void On_buttonFaces_Clicked();
    void On_buttonOperations_Clicked();
    void On_buttonOpMatrix_Clicked();
    void On_buttonAddOp_Clicked();
    void On_buttonRemoveOp_Clicked();
    void On_buttonOpUp_Clicked();
    void On_buttonOpDown_Clicked();
    void On_buttonOpBack_Clicked();
    void On_buttonSetOpParam_Clicked();
    void On_buttonClearConsole_Clicked();
    void On_buttonVerticesFromFace_Clicked();
    void On_buttonFacesFromEdge_Clicked();
    void On_buttonEdgesFromVertices_Clicked();

signals:
    void OpenObj(QString filename);
    void GetSelectedVertice(const Vertice *v);
    void GetSelectedEdge(const Edge *e);
    void GetSelectedFace(const Face *f);
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
    void ViewportProportion(int width, int height);
};
#endif // MAINWINDOW_H
