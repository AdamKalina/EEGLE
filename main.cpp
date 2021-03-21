#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "czech");
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("EEGle");
    //w.setWindowState(Qt::WindowMaximized);
    w.setMinimumHeight(820);
    w.setMinimumWidth(1200);
    w.show();
    return a.exec();
}
