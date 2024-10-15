#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

private:
    Ui::MainWindow *_ui;

    void ConsoleMessage(QString msg);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool Init();
};
#endif // MAINWINDOW_H
