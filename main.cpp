#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QStyleFactory>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;

    a.setStyle(QStyleFactory::create("Windows"));

    if(!w.Init()){
        QMessageBox::warning(nullptr, "Erro!", "Falha de inicialização!\n O programa será fechado.");
        return 0;
    }

    w.showMaximized();

    return a.exec();
}
