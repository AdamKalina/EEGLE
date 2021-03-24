#include "mainwindow.h"

MainWindow::~MainWindow()
{
    delete maincurve;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint); // disable resizing by hand


    // load data
    // TO DO - samostatna metoda na otevirani souboru
    string path2file = "D:/Dropbox/Share/B0000938.sig";
    signal = read_signal_file(path2file, true);
    files_open = 1; // 1 if file is loaded


    qDebug() << "number of non-zero samples: " << signal.signal_data[1].size() - std::count(signal.signal_data[1].begin(), signal.signal_data[1].end(), 0);
    qDebug() << "epoch length in samples: " << signal.recorder_info.epochLengthInSamples;
    qDebug() << "num of pages: " << signal.signal_pages.size();
    qDebug() << "length in seconds: " << (signal.recorder_info.epochLengthInSamples/250)*signal.signal_pages.size();
    //lengthOfFile = (signal.recorder_info.epochLengthInSamples/250)*signal.signal_pages.size();
    lengthOfFile = (signal.signal_data[1].size() - std::count(signal.signal_data[1].begin(), signal.signal_data[1].end(), 0))/signal.recorder_info.channels[1].sampling_rate;//number of non-zero samples/sampling frequency
    qDebug() <<  lengthOfFile;

    // variables for diplaying EEG
    viewtime = 0; //start of left edge of viewed page in seconds
    pagetime = 5; // number of seconds to show on the screen
    mouseWheel = 0; //1 = step, 0 = page

    test_patinfo(&signal.measurement);

    foo = 88;

    //Deprecated - usable only for private and protected variable
    //setFoo(foo);
    //HDR = signal.measurement;
    //setHDR(signal.measurement);
    //setSignal(signal);
    qDebug()<< "Foo v mainwindow: " <<foo;
    qDebug() << signal.recorder_info.epochLengthInSamples;

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

    helpmenu = new QMenu(this);
    helpmenu->setTitle("&About");
    helpmenu->addAction("About", this, SLOT(show_about_dialog()));
    helpmenu->addAction("Keyboard shortcuts", this, SLOT(show_kb_shortcuts()));
    menubar->addMenu(helpmenu);

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

void MainWindow::show_kb_shortcuts()
{
    QMessageBox messagewindow(QMessageBox::NoIcon,
                              "Keyboard shortcuts",

                              "Nic One Style - default\n"
   "PgDn\t\tnext page\n"
   "PgUp\t\tformer page\n"
   "Right Arrow\tshift right one second\n"
   "Left Arrow\tshift left one second\n"
"\n"

    "BrainLab Style - optional\n"
    "Right Arrow\tnext page\n"
    "Left Arrow\tprevious page\n"
   "PgDn\t\tnext page\n"
   "PgUp\t\tformer page\n"
"\n"

    "Common\n"
   "Up Arrow\tincrease amplitude\n"
   "Down Arrow\tdecrease amplitude\n"
   "Home\t\tgo to start of file\n"
   "End\t\tgo to end of file\n"
   "1 - 8\t\tload predefined montage\n"
   "\nCtrl+O\t\tOpen a file\n"
   "Alt+F4\t\tExit program\n"

   "\nMousewheel\tshift left or right\n"

   "\nSpace\t\tToggle Playback or Pause\n"
  );

    messagewindow.exec();
}

void MainWindow::shift_page_right()
{

    if(!files_open)  return;

    if(viewtime >= lengthOfFile - pagetime) return;

    viewtime += 1;
    qDebug() << "viewtime: " << viewtime;

    maincurve->update();
}

void MainWindow::shift_page_left()
{

    if(!files_open)  return;

    if(viewtime == 0) return;

    viewtime -= 1;
    qDebug() << "viewtime: " << viewtime;

    maincurve->update();
}

void MainWindow::next_page()
{

    if(!files_open)  return;

    if(viewtime >= lengthOfFile - pagetime) return;

    viewtime += pagetime;
    qDebug() << "viewtime: " << viewtime;

    maincurve->update();
}

void MainWindow::previous_page()
{

    if(!files_open)  return;
    if(viewtime == 0) return;

    if(viewtime <= pagetime){
        viewtime = 0;
    }
    else{
        viewtime -= pagetime;
    }
    qDebug() << "viewtime: " << viewtime;

    maincurve->update();
}

void MainWindow::first_page()
{

    if(!files_open)  return;
    if(viewtime == 0) return;

    viewtime = 0;

    maincurve->update();
}

void MainWindow::last_page()
{

    if(!files_open)  return;

    viewtime = lengthOfFile - pagetime;

    maincurve->update();
}




