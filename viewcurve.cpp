#include "viewcurve.h"


ViewCurve::ViewCurve(QWidget *w_parent) : QWidget(w_parent)
{

    //setAttribute(Qt::WA_OpaquePaintEvent); // to bych pak musel nastavovat barvy

    mainwindow = (MainWindow *)w_parent;

    shift_page_left_shortcut = NULL;
    shift_page_right_shortcut = NULL;
    next_page_shortcut = NULL;
    previous_page_shortcut = NULL;

    arrowkeys_shortcuts_global_set_enabled(true);

    QScreen *screen = QGuiApplication::primaryScreen();
    qDebug() << "QScreen geometry height in px: " << screen->geometry().height();
    qDebug() <<  "QScreen geometry width in px" << screen->geometry().width();
    qDebug() << "Screen's physical size in mm: " << screen->physicalSize();
    qDebug() << "Screen's physical height in mm: " << screen->physicalSize().height(); // screen's physical size (in millimeters)
    qDebug() << "Screen's physical width in mm: " << screen->physicalSize().width();
    qDebug() << "Physical dots per mm X: " << screen->geometry().width()/screen->physicalSize().width();
    qDebug() << "Physical dots per mm Y: " << screen->geometry().height()/screen->physicalSize().height();
    qDebug() << "Physical dots per inch: " << screen->physicalDotsPerInch(); //number of physical dots or pixels per inch
    qDebug() << "Physical dots per mm: " << screen->physicalDotsPerInch()*0.0393701; //number of physical dots or pixels per inch

    // BrainLab window 310x260 mm

    //qDebug() << "window height: " << mainwindow->size().height();
    //qDebug() << "window width: " << mainwindow->size().width();

    //qDebug() <<"Foo ve viewcurve: " << mainwindow->foo;
}


void ViewCurve::arrowkeys_shortcuts_global_set_enabled(bool enabled)
{
    //    if(shift_page_left_shortcut != NULL)
    //    {
    //        QObject::disconnect(shift_page_left_shortcut, 0, 0, 0);
    //        delete shift_page_left_shortcut;
    //    }

    //    if(shift_page_right_shortcut != NULL)
    //    {
    //        QObject::disconnect(shift_page_right_shortcut, 0, 0, 0);
    //        delete shift_page_right_shortcut;
    //    }

    //    if(next_page_shortcut != NULL)
    //    {
    //        QObject::disconnect(next_page_shortcut, 0, 0, 0);
    //        delete next_page_shortcut;
    //    }

    //    if(previous_page_shortcut != NULL)
    //    {
    //        QObject::disconnect(previous_page_shortcut, 0, 0, 0);
    //        delete previous_page_shortcut;
    //    }

    // TO DO - switching between BrainLab and NicOne style

    if(enabled == true)
    {
        shift_page_left_shortcut = new QShortcut(QKeySequence::MoveToPreviousChar, mainwindow);
        shift_page_right_shortcut = new QShortcut(QKeySequence::MoveToNextChar, mainwindow);
        next_page_shortcut = new QShortcut(QKeySequence::MoveToNextPage, mainwindow);
        previous_page_shortcut = new QShortcut(QKeySequence::MoveToPreviousPage, mainwindow);
        first_page_shortcut = new QShortcut(QKeySequence::MoveToStartOfLine, mainwindow);
        last_page_shortcut = new QShortcut(QKeySequence::MoveToEndOfLine, mainwindow);
    }
    else
    {
        shift_page_left_shortcut = new QShortcut(QKeySequence::MoveToPreviousChar, this, 0, 0, Qt::WidgetShortcut);
        shift_page_right_shortcut = new QShortcut(QKeySequence::MoveToNextChar, this, 0, 0, Qt::WidgetShortcut);
        next_page_shortcut = new QShortcut(QKeySequence::MoveToNextPage, this, 0, 0, Qt::WidgetShortcut);
        previous_page_shortcut = new QShortcut(QKeySequence::MoveToPreviousPage, this, 0, 0, Qt::WidgetShortcut);
        first_page_shortcut = new QShortcut(QKeySequence::MoveToStartOfLine, this, 0, 0, Qt::WidgetShortcut);
        last_page_shortcut = new QShortcut(QKeySequence::MoveToEndOfLine, this, 0, 0, Qt::WidgetShortcut);
    }

    QObject::connect(shift_page_left_shortcut,  SIGNAL(activated()), mainwindow, SLOT(shift_page_left()));
    QObject::connect(shift_page_right_shortcut, SIGNAL(activated()), mainwindow, SLOT(shift_page_right()));
    QObject::connect(next_page_shortcut,  SIGNAL(activated()), mainwindow, SLOT(next_page()));
    QObject::connect(previous_page_shortcut, SIGNAL(activated()), mainwindow, SLOT(previous_page()));
    QObject::connect(first_page_shortcut, SIGNAL(activated()), mainwindow, SLOT(first_page()));
    QObject::connect(last_page_shortcut, SIGNAL(activated()), mainwindow, SLOT(last_page()));
}

//
void ViewCurve::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);


    // couple of variable to use when drawing
    int fromTop = 50;
    int offset = 30;
    int second2draw = mainwindow->pagetime;
    std::string label;
    QString qstr;
    int LeftMargin = 50;
    //float pixelFactor = 0.005;
    int scaleY = 50;

    // viewbuf
    // TO DO - move it to separate class for resampling, filtering, montages etc
    std::vector<std::vector<double>> viewbuf;


    if (mainwindow->notch){
        double fs = 250;
        int order = 1;
        float frequency = 50;
        char *filter_spec;
        char spec_str_1[256];
        char *err;


        // ====== FILTERING ======
        //FidFilter *ff;
        //free(fbuf1);
        //fid_run_free(run);
        //free(ff);

        filter_spec = spec_str_1;
        spec_str_1[0] = 0;

        _snprintf(spec_str_1, 256, "BsRe/%i/%f", order, frequency);
        qDebug() << spec_str_1;
        //err= fid_parse(double rate, char **pp, FidFilter **ffp);
        //err = fid_parse(rate, &filter_spec, &ff);
            //err = fid_parse(fs,&filter_spec, &ff);

        if(err != NULL)
          {
            QMessageBox messagewindow(QMessageBox::Critical, "Error", err);
            messagewindow.exec();
            free(err);
            return;
          }
        else{
            qDebug() << "Filter created!";
        }

        //run= fid_run_new(ff, &funcp);
        //fbuf1= fid_run_newbuf(run);
    }

    // ====== LOAD DATA TO VIEWBUF ======
    if (mainwindow->signal.signal_data.empty()) return;

    for (int i = 0; i < mainwindow->signal.recorder_info.numberOfChannelsUsed;i++){
        if (i >= (int)mainwindow->signal.signal_data.size()) break;

        int timeWindow = mainwindow->signal.recorder_info.channels[i].sampling_rate*second2draw;
        long long windowStart = mainwindow->viewtime*mainwindow->signal.recorder_info.channels[i].sampling_rate;

        if (windowStart < 0) windowStart = 0;
        if (windowStart >= (long long)mainwindow->signal.signal_data[i].size()) {
            viewbuf.push_back(std::vector<double>());
            continue;
        }

        if (windowStart + timeWindow > (long long)mainwindow->signal.signal_data[i].size()) {
            timeWindow = (long long)mainwindow->signal.signal_data[i].size() - windowStart;
        }

        std::vector<double> temp_channel;
        if (timeWindow > 0) {
            temp_channel.assign(mainwindow->signal.signal_data[i].begin()+windowStart, mainwindow->signal.signal_data[i].begin() + windowStart + timeWindow);
        }

        // filtering
        if (mainwindow->notch){
//        for (int j = 0; j < temp_channel.size(); j++){
//            temp_channel[j]= funcp(fbuf1, temp_channel[j]);
//        }
//        fid_run_zapbuf(fbuf1);
        }

        viewbuf.push_back(temp_channel);
    }


    // ====== painter definition ======
    QPen signal_pen(Qt::black, 1, Qt::SolidLine,Qt::SquareCap, Qt::BevelJoin);
    QPen grid_pen(Qt::gray, 1, Qt::SolidLine,Qt::SquareCap, Qt::BevelJoin);

    // and some font
    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);

    // TO DO - split grid, signals and labels to different processes
    // ====== DRAW GRIDS ======
    for (int i = 1; i < second2draw; i++){
        // TO DO - replace 250 with variable - e.g. signal.recorder_info.channels[i].sampling_rate
        painter.setPen(grid_pen);
        painter.drawLine(LeftMargin + i*250,0,LeftMargin + i*250,1000);
    }


    // ====== DRAW SIGNALS =======
    for (int i = 0; i < (int)viewbuf.size(); i++){
        painter.setPen(grid_pen);
        painter.drawLine(LeftMargin,fromTop + offset*i,1000+LeftMargin,fromTop + offset*i);
        painter.setPen(signal_pen);
        label = mainwindow->signal.recorder_info.channels[i].channel_desc;
        qstr = QString::fromStdString(label);
        painter.drawText(1, fromTop+offset*i, qstr);
        int fromX = LeftMargin;
        int fromY = fromTop + offset*i;

        // draw signals from viewbuf
        if (viewbuf[i].size() > 1) {
            for (int j = 0; j < (int)viewbuf[i].size() - 1; j++){
                painter.drawLine(fromX+j, fromY-viewbuf[i][j]*scaleY, fromX+j+1, fromY-viewbuf[i][j+1]*scaleY);
            }
        }
    }

    //draw time!
    // TO DO - move it to separate widget
    painter.drawText(1, fromTop+offset*mainwindow->signal.recorder_info.numberOfChannelsUsed, QString::fromStdString("Time:"));
}

void ViewCurve::wheelEvent(QWheelEvent *wheel_event)
{

    if(mainwindow->file_open == 0)  return;
    //qDebug() << wheel_event->angleDelta().y();
    if(wheel_event->angleDelta().y() > 0){
        if (mainwindow->mouseWheel == 1)  mainwindow->shift_page_left();
        if (mainwindow->mouseWheel == 0)  mainwindow->previous_page();
    }
    if(wheel_event->angleDelta().y() < 0){
        if (mainwindow->mouseWheel == 1)  mainwindow->shift_page_right();
        if (mainwindow->mouseWheel == 0)  mainwindow->next_page();
    }

}


ViewCurve::~ViewCurve()
{
    delete signal_pen;
    delete grid_pen;
}
