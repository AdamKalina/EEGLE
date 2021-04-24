#include "mainwindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[]) // argc = argument cout, argv = arguments
{
    setlocale(LC_ALL, "czech");
    QApplication app(argc, argv);

    //    for (int i = 1; i < argc; ++i) {
    //            std::cout << "The " << i << "th argument is " << argv[i] << std::endl;
    //        }

    qDebug() << app.arguments().size();
    if(app.arguments().size() == 2){
        //qDebug() << "argv" << app.arguments().at(1).toLocal8Bit().data();
    }

    MainWindow w;
    w.setWindowTitle("EEGLE");
    //w.setWindowState(Qt::WindowMaximized);
    w.setWindowIcon(QIcon(":/images/eagle_icon.png"));
    w.setMinimumHeight(820);
    w.setMinimumWidth(1200);

    QTimer::singleShot(0, & w, SLOT(initialize())); // delays the parsing of command line argument after the mainwindow is loaded

    // define the splash screen
    // relative path using https://doc.qt.io/qt-5/resources.html
    // TO DO - put the version into common variable
    QPixmap pixmap(":images/eagle.png");

    QPainter p(&pixmap);
    QFont sansFont("Century Gothic", 10);
    p.setFont(sansFont);
    p.setPen(Qt::black);
    p.drawText(200, 340, 300, 30, Qt::AlignLeft | Qt::TextSingleLine, "version 0.2");

    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);

    QTimer t1;
    t1.setSingleShot(true);
    QObject::connect(&t1, SIGNAL(timeout()), &splash, SLOT(close()));
    splash.show();
    t1.start(1500); // show the splash screen for 1500 ms
    QEventLoop evlp;
    QTimer::singleShot(100, &evlp, SLOT(quit()));
    evlp.exec();

    w.show();
    return app.exec();
}
