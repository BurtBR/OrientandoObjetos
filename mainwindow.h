#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    Ui::MainWindow *_ui;
    QThread *_threadFileHandler = nullptr;

    void DeleteThread(QThread **thread);
    void ConsoleMessage(QString msg);
    bool StartThreadFileHandler();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool Init();

private slots:
    void WorkerMessage(QString msg);
    void FileHandlingFinished();

    void On_actionAbrir_triggered(bool);

signals:
    void OpenFile(QString filename);
};
#endif // MAINWINDOW_H
