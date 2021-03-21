#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // load data
    string path2file = "D:/Dropbox/Share/B0000938.sig";

    signal = read_signal_file(path2file, true);

    test_patinfo(&signal.measurement);

    foo = 88;
    //setFoo(foo);

    HDR = signal.measurement;

    //setHDR(signal.measurement);
    //setSignal(signal);
    qDebug()<< "Foo v mainwindow: " <<foo;

    //maincurve
    maincurve = new ViewCurve(this);
    setCentralWidget(maincurve);

    //menus
    menubar = menuBar();

    filemenu = new QMenu(this);
    filemenu->setTitle("&File");
    menubar->addMenu(filemenu);

    infomenu = new QMenu(this);
    infomenu->setTitle("&Info");
    infomenu->addAction("Patient info", this, SLOT(show_patient_info()));
    menubar->addMenu(infomenu);

    aboutmenu = new QMenu(this);
    aboutmenu->setTitle("&About");
    aboutmenu->addAction("About", this, SLOT(show_about_dialog()));
    menubar->addMenu(aboutmenu);
}

void MainWindow::test_patinfo(Measurement *patinfo){
    cout << "TEST" << endl;
    cout << "id: " << patinfo->id << endl;
    cout << "name: " << patinfo->name << endl;
    cout << "doctor: " << patinfo->doctor << endl;
    cout << "technician: " << patinfo->technician << endl;
    cout << "sex: " << patinfo->sex << endl;
    time_t epoch = decode_date_time(patinfo->start_date, patinfo->start_hour);
    cout << ctime(&epoch) << endl;
}

void MainWindow::show_about_dialog()
{
    QMessageBox messagewindow(QMessageBox::NoIcon,
                              "About this program",
                              "EEGle is an EEG reader created as a substitution for the old BrainLab reader running only on Windows XP.\n"
    "\n"
    "EEGle reader is inspired by EDF Browser from Teuniz and is using convertSIGtoEDF from Frederik-D-Weber to read BrainLab EEG files\n"
    "\n"
    "Built using Qt Creator 4.14.1 and Qt 5.15.2 (MSVC 2019, 64 bit)"
    "\n"
    "\n"
    "by Adam Kalina, Department of Neurology, Second Faculty of Medicine, Charles University and Motol University Hospital, 2021, during COVID-19"
  );
    messagewindow.setStyleSheet("QLabel{min-width: 700px;}");
    messagewindow.exec();
}

void MainWindow::show_patient_info()
{
    hdrwindow hdrwindow(this);
}

MainWindow::~MainWindow()
{
    delete maincurve;
}


