/// @file main.cpp
/// @author Yann Di Padova

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;

    // Icone et nom d'application
    w.setWindowTitle("Roboto Collaboratif");
    w.setWindowIcon(QIcon(":/icon/icone.ico"));
    w.showMaximized();

    return a.exec();
}
