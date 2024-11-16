#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QThread>
#include "errormessage.h"

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

    void On_listVertices_SelectionChanged(int idx);
    void On_actionAbrir_triggered(bool);
    void On_buttonVertices_Clicked();
    void On_buttonEdges_Clicked();
    void On_buttonFaces_Clicked();
    void On_buttonClearConsole_Clicked();

signals:
    void OpenObj(QString filename);
    void PrintAllData();
};
#endif // MAINWINDOW_H
