#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QThread>
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
    void SetVerticeList(QStringList list);
    void SetEdgeList(QStringList list);
    void SetFaceList(QStringList list);
    void SetSelectedVerticeData(Vertice v);
    void SetSelectedEdgeData(Edge e);
    void SetSelectedFaceData(Face f);
    void SetOperationMatrix(QMatrix4x4 M);
    void SetOperationList(QStringList list);

    void On_listVertices_SelectionChanged(int idx);
    void On_listEdges_SelectionChanged(int idx);
    void On_listFaces_SelectionChanged(int idx);
    void On_actionAbrir_triggered(bool);
    void On_buttonVertices_Clicked();
    void On_buttonEdges_Clicked();
    void On_buttonFaces_Clicked();
    void On_buttonOperations_Clicked();
    void On_buttonOpMatrix_Clicked();
    void On_buttonOpBack_Clicked();
    void On_buttonClearConsole_Clicked();
    void On_buttonVerticesFromFace_Clicked();
    void On_buttonFacesFromEdge_Clicked();
    void On_buttonEdgesFromVertices_Clicked();

signals:
    void OpenObj(QString filename);
    void GetSelectedVertice(size_t id);
    void GetSelectedEdge(size_t id);
    void GetSelectedFace(size_t id);
    void PrintAllData();
    void PrintVerticesFromFace(size_t f);
    void PrintFacesFromEdge(size_t e);
    void PrintEdgesFromVertice(size_t v);
    void AddOperation(Operation op);
    void MoveOperationUp(size_t idx);
    void MoveOperationDown(size_t idx);
};
#endif // MAINWINDOW_H
