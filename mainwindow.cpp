#include "mainwindow.h"

MainWindow::~MainWindow()
{
    delete maincurve;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint); // disable resizing by hand
    setAcceptDrops(true);

    // variables for diplaying EEG
    lengthOfFile = (signal.recorder_info.epochLengthInSamples/250)*signal.signal_pages.size();

    //Deprecated - usable only for private and protected variable
    //foo = 88;
    //setFoo(foo);
    //HDR = signal.measurement;
    //setHDR(signal.measurement);
    //setSignal(signal);
    //qDebug()<< "Foo v mainwindow: " <<foo;


    //maincurve
    maincurve = new ViewCurve(this);
    setCentralWidget(maincurve);

    //menus
    createToolbars();


    menubar = menuBar();

    // ======== File menu ========
    filemenu = new QMenu(this);
    filemenu->setTitle("&File");
    filemenu->addAction("Open", this, SLOT(open_file_dialog()));
    filemenu->addAction("Patient info", this, SLOT(show_patient_info()));
    menubar->addMenu(filemenu);


    // ======== Info ======== // not used right now
    infomenu = new QMenu(this);
    infomenu->setTitle("&Info");
    menubar->addMenu(infomenu);


    // ======== Timescale ========
    //define actions
    BLdefaultAction = new QAction(tr("xxx s/cm (BrainLab default)"), this);
    BLdefaultAction->setCheckable(true);

    fiveCmPerSec = new QAction(("5 cm/s"),this);
    fiveCmPerSec->setCheckable(true);

    twoAndHalfCmPerSec = new QAction(("2.5 cm/s"),this);
    twoAndHalfCmPerSec->setCheckable(true);

    fiveSecPerPageAction = new QAction(tr("5 s/page"), this);
    fiveSecPerPageAction->setCheckable(true);

    tenSecPerPageAction = new QAction(tr("10 s/page"),this);
    tenSecPerPageAction->setCheckable(true);

    fifteenSecPerPageAction = new QAction(tr("15 s/page"),this);
    fifteenSecPerPageAction->setCheckable(true);

    twentySecPerPageAction = new QAction(tr("20 s/page"),this);
    twentySecPerPageAction->setCheckable(true);

    thirtySecPerPageAction = new QAction(tr("30 s/page"), this);
    thirtySecPerPageAction->setCheckable(true);


    // create the group to make options mutually exclusive
    timescalegroup = new QActionGroup(this);
    timescalegroup->addAction(BLdefaultAction);
    timescalegroup->addAction(twoAndHalfCmPerSec);
    timescalegroup->addAction(fiveCmPerSec);
    timescalegroup->addAction(fiveSecPerPageAction);
    timescalegroup->addAction(tenSecPerPageAction);
    timescalegroup->addAction(fifteenSecPerPageAction);
    timescalegroup->addAction(twentySecPerPageAction);
    timescalegroup->addAction(thirtySecPerPageAction);

    // construct the menu
    timemenu = new QMenu(this);
    timemenu->setTitle("&Timescale");
    timemenu->addAction("Fixed resolution");
    timemenu->addAction(BLdefaultAction);
    timemenu->addAction(twoAndHalfCmPerSec);
    timemenu->addAction(fiveCmPerSec);
    timemenu->addSeparator();
    timemenu->addAction("Fixed page range");
    timemenu->addAction(fiveSecPerPageAction);
    timemenu->addAction(tenSecPerPageAction);
    timemenu->addAction(fifteenSecPerPageAction);
    timemenu->addAction(twentySecPerPageAction);
    timemenu->addAction(thirtySecPerPageAction);
    menubar->addMenu(timemenu);

    // ======== Amplitude ========
    amplitudemenu = new QMenu(this);
    amplitudemenu->setTitle("&Amplitude");
    menubar->addMenu(amplitudemenu);

    // ======== Montages ========
    montagemenu = new QMenu(this);
    montagemenu->setTitle("&Montage");
    // auto-generate these
    menubar->addMenu(montagemenu);


    // ======== Settings ========
    // settings menu - TO DO - separators with text, e.g. https://stackoverflow.com/questions/22635903/non-interactive-items-in-qmenu
    // TO DO - make text in separators bold
    settingsmenu= new QMenu(this);
    settingsmenu->setTitle("&Settings");

    // ======== Keyboard bindings ========
    //settingsmenu->addSection("Keyboard bindings"); // does not work on windows? differnt widget style?
    //settingsmenu->addSeparator()->setText("Keyboard bindings"); // does not work on windows?
    settingsmenu->addAction(tr("Keyboard bindings"));

    // create separate action for each possibility and register them to groups (so they are mutualy exclusive) and then to menu
    BrainLabAction = new QAction(tr("BrainLab"), this);
    BrainLabAction->setCheckable(true);

    NicOneAction = new QAction(tr("NicOne"), this);
    NicOneAction->setCheckable(true);
    NicOneAction->setChecked(true);

    //settingsmenu->addAction("BrainLab")->setCheckable(true); // simpler way, but then unable to reigster in group
    //settingsmenu->addAction("NicOne")->setCheckable(true);
    // create group and register methods
    keyboardgroup = new QActionGroup(this);
    keyboardgroup->addAction(BrainLabAction);
    keyboardgroup->addAction(NicOneAction);
    // add methods to menu
    settingsmenu->addAction(BrainLabAction);
    settingsmenu->addAction(NicOneAction);

    // ======== Mouse wheel scrolling ========
    settingsmenu->addSeparator();
    settingsmenu->addAction("Mouse wheel scrolling");

    mousePageAction = new QAction(tr("Page"), this);
    mousePageAction->setCheckable(true);
    mousePageAction->setChecked(true);

    mouseStepAction = new QAction(tr("Step"), this);
    mouseStepAction->setCheckable(true);


    mousewheelgroup = new QActionGroup(this);
    mousewheelgroup->addAction(mousePageAction);
    mousewheelgroup->addAction(mouseStepAction);
    settingsmenu->addAction(mousePageAction);
    settingsmenu->addAction(mouseStepAction);
    menubar->addMenu(settingsmenu);

    connect(mousePageAction,SIGNAL(triggered()),this,SLOT(mousewheel_mode_page()));
    connect(mouseStepAction,SIGNAL(triggered()),this,SLOT(mousewheel_mode_step()));

    // ======== Help menu ========
    helpmenu = new QMenu(this);
    helpmenu->setTitle("&Help");
    helpmenu->addAction("About", this, SLOT(show_about_dialog()));
    helpmenu->addAction("Keyboard shortcuts", this, SLOT(show_kb_shortcuts()));
    menubar->addMenu(helpmenu);
}

void MainWindow::test_patinfo(read_signal_file::Measurement *patinfo){
    std::cout << "TEST" << endl;
    std::cout << "id: " << patinfo->id << endl;
    std::cout << "name: " << patinfo->name << endl;
    std::cout << "doctor: " << patinfo->doctor << endl;
    std::cout << "technician: " << patinfo->technician << endl;
    std::cout << "sex: " << patinfo->sex << endl;
    QDateTime epoch = decode_date_time(patinfo->start_date, patinfo->start_hour);
}


void MainWindow::initialize()
{
    QStringList arguments = QCoreApplication::arguments();
    // Now you can parse the arguments *after* the main window has been created.
    //qDebug() << arguments[1];
    //qDebug() << arguments.size();
    // accept only one arguement from the command line
    if(arguments.size() == 2){
        open_file(arguments[1].toLocal8Bit().data());
    }

}

void MainWindow::open_file_dialog(){

    // use toLocal8Bit() for converting QString to std::string
    path2file = QFileDialog::getOpenFileName(this, tr("Open BrainLab EEG file"), "D:/Dropbox/Script/Cpp", tr("BrainLab Files (*.sig)")).toLocal8Bit().data();

    if(path2file.empty()){
        return;
    }

    open_file(path2file);
};


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    // accept files only to being dragged over the mainwindow
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

//void MainWindow::dropEvent(QDropEvent* event)
// {
//   const QMimeData* mimeData = event->mimeData();

//   // check for our needed mime type, here a file or a list of files
//   if (mimeData->hasUrls())
//   {
//     QStringList pathList;
//     QList<QUrl> urlList = mimeData->urls();

//     // extract the local paths of the files
//     for (int i = 0; i < urlList.size() && i < 32; +i)
//     {
//       pathList.append(urlList.at(i).toLocalFile());
//     }

//     // call a function to open the files
//     //open_file(pathList);
//     qDebug() << pathList;
//   }
// }


void MainWindow::dropEvent(QDropEvent* e)
{
    // TO DO - accepty only the first file when dropped multiple (or open them in new window?)
    // TO DO - maincurve is not redrawn until you click in the mainwindow
    QStringList accepted_types;
    accepted_types << "sig";
    foreach(const QUrl & url, e->mimeData()->urls()){
        QString fname = url.toLocalFile();
        QFileInfo info(fname);
        if (info.exists())
        {
            if (accepted_types.contains(info.suffix().trimmed(), Qt::CaseInsensitive))
                // do whatever you need to do with fname variable
                //qDebug() << url;
                qDebug() << fname;
            open_file(fname.toLocal8Bit().data());
        }
    }
}

void MainWindow::open_file(std::string path2file){

    QFileInfo infoSig(QString::fromStdString(path2file));
    read_signal_file signalReader;
    signal = signalReader.read_signal_file_all(infoSig, false);

    // TO DO - fallbacks for eror when file is not valid, is empty etc.
    // TO DO - open dropped files --> maincurve is not updated until focus is back on EEGle
    // TO DO - update maincurve? delete it first?

    if (!signal.check) {
        return;
    }


    // 1. Clean up old manager if we are opening a new file
    if (m_dataManager) {
        delete m_dataManager;
    }

    file_open = 1; // 1 if file is loaded

    if (signal.recorder_info.highestRate > 0) {
        lengthOfFile = (signal.recorder_info.epochLengthInSamples / (double)signal.recorder_info.highestRate) * signal.signal_pages.size();
    }

    // 2. Create the new manager
    m_dataManager = new EegDataManager(&signal);

    // 3. Tell the ViewCurve to use this manager
    maincurve->setDataManager(m_dataManager, signal.recorder_info.channels);

    // 4. Set the initial view (e.g., start at 0 seconds, show 10 seconds)
    maincurve->setTimeWindow(0.0, 10.0);
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
    if (file_open){
        hdrwindow hdrwindow(this);
    }
    else{
        return;
    }

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
    if(!file_open)  return;

    if(viewtime >= lengthOfFile - pagetime) return;

    viewtime += 1;
    qDebug() << "viewtime: " << viewtime << " s";

    maincurve->update();
}

void MainWindow::shift_page_left()
{
    if(!file_open)  return;

    if(viewtime == 0) return;

    viewtime -= 1;
    qDebug() << "viewtime: " << viewtime << " s";

    maincurve->update();
}

void MainWindow::next_page()
{
    if(!file_open)  return;

    if(viewtime >= lengthOfFile - pagetime) return;

    viewtime += pagetime;
    qDebug() << "viewtime: " << viewtime << " s";
    maincurve->update();
}

void MainWindow::previous_page()
{
    if(!file_open)  return;
    if(viewtime == 0) return;

    if(viewtime <= pagetime){
        viewtime = 0;
    }
    else{
        viewtime -= pagetime;
    }
    qDebug() << "viewtime: " << viewtime << " s";

    maincurve->update();
}

void MainWindow::first_page()
{
    if(!file_open)  return;
    if(viewtime == 0) return;
    viewtime = 0;
    maincurve->update();
}

void MainWindow::last_page()
{
    if(!file_open)  return;
    qDebug() << "last_page";
    viewtime = lengthOfFile - pagetime;
    maincurve->update();
}


// TO DO - find ho to do this in one step using one slot
void MainWindow::mousewheel_mode_page(){
    qDebug() << "mouse wheel mode = page";
    this->mouseWheelMode = 0;
}

void MainWindow::mousewheel_mode_step(){
    qDebug() << "mouse wheel mode = step";
    this->mouseWheelMode = 1;
}

void MainWindow::createToolbars(){
    QToolBar *toolBar = addToolBar(tr("Toolbar"));
    notchButton = new QAction(tr("Notch"), this);
    notchButton->setCheckable(true);
    notchButton->setChecked(false);
    toolBar->addAction(notchButton);
    connect(notchButton, SIGNAL(toggled(bool)), this, SLOT(notchToggle(bool)));

    QLabel *LFFLabel = new QLabel(tr("LFF: "));
    QSpinBox *LFFspinBox = new QSpinBox(this); // automatic range = 0-99

    toolBar->addWidget(LFFLabel);
    toolBar->addWidget(LFFspinBox);

    QLabel *HFFLabel = new QLabel(tr("  HFF: "));
    QSpinBox *HFFspinBox = new QSpinBox(this);
    HFFspinBox->setSingleStep(5);
    toolBar->addWidget(HFFLabel);
    toolBar->addWidget(HFFspinBox);
}

void MainWindow::notchToggle(bool checked)
{
    if (checked) {
        // Examine the new button states.
        notch = 1;
        qDebug() << notch;
        maincurve->update();
    }
    else{
        notch = 0;
        qDebug() << notch;
        maincurve->update();
    }
}
