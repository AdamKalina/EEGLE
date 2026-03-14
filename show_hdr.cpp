#include "show_hdr.h"

hdrwindow::hdrwindow(QWidget *w_parent)
{
    mainwindow = (MainWindow *)w_parent;

    // zkouska
    //qDebug() << "test primeho spojeni: " << mainwindow->foo;
    //int bar = mainwindow->getFoo();
    //qDebug() << "test spojeni pres getter: " << bar;


    //Measurement HDR = mainwindow->HDR; // prime spojeni + deklarace
    //Measurement HDR = mainwindow->getHDR(); //funguje, ale zrejme, kdyz je vytvoreny setter, tak ani nemusim volat getter
    //qDebug() << HDR.id;


    myfont = new QFont;
    myfont->setFamily("Tahoma");
    myfont->setPixelSize(21);
    //QApplication::setFont(*myfont); // changes it everywhere

    QTextCodec *codec = QTextCodec::codecForName("Windows-1250");
    AboutDialog = new QDialog;
    AboutDialog->setMinimumSize(640, 480);
    AboutDialog->setGeometry(300, 250, 640, 480);
    AboutDialog->setWindowTitle("Patient info");
    AboutDialog->setModal(true);
    AboutDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    AboutDialog->setSizeGripEnabled(true);

    QLabel *label[7];
    label[0] = new QLabel (QString("ID: %1").arg(mainwindow->signal.measurement.id));
    QString name = codec->toUnicode(mainwindow->signal.measurement.name);
    label[1] = new QLabel (QString("Name: %1").arg(name));
    QString doctor = codec->toUnicode(mainwindow->signal.measurement.doctor);
    label[2] = new QLabel (QString("Doctor: %1").arg(doctor));
    QString technician = codec->toUnicode(mainwindow->signal.measurement.technician);
    label[3] = new QLabel (QString("Technician: %1").arg(technician));
    label[4] = new QLabel (QString("Sex: %1").arg(mainwindow->signal.measurement.sex));
    QDateTime qtime = decode_date_time(mainwindow->signal.measurement.start_date, mainwindow->signal.measurement.start_hour);
    //qDebug() << ctime(&epoch);
    //qDebug() << qtime;
    label[5] = new QLabel (QString("Start time: %1").arg(qtime.toString("dd. MM. yyyy hh:mm")));
    label[6] = new QLabel (QString("Recorder name: %1").arg(mainwindow->signal.recorder_info.name));

    pushButton1 = new QPushButton;
    QString close = codec->toUnicode("Zavøít");
    pushButton1->setText(close);

    //TO DO: simplify this layout
    hlayout1 = new QHBoxLayout;
    hlayout1->insertStretch(0, 100);
    hlayout1->addWidget(pushButton1);

    vlayout1 = new QVBoxLayout;

    for (int i=0;i<7;i++) {
        label[i]->setFont(*myfont);
        vlayout1->addWidget(label[i]);
    }


    //vlayout1->setStretchFactor(textedit1, 100);
    vlayout1->addLayout(hlayout1);
    //vlayout1->setStretchFactor(hlayout1, 0);

    AboutDialog->setLayout(vlayout1);

    QObject::connect(pushButton1, SIGNAL(clicked()), AboutDialog, SLOT(close()));

    AboutDialog->exec();
}
