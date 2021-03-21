#include "viewcurve.h"


ViewCurve::ViewCurve(QWidget *w_parent) : QWidget(w_parent)
{

    //setAttribute(Qt::WA_OpaquePaintEvent); // to bych pak musel nastavovat barvy

    mainwindow = (MainWindow *)w_parent;

    qDebug() <<"Foo ve viewcurve: " << mainwindow->foo;
}


void ViewCurve::paintEvent(QPaintEvent *event)
{
    //create a QPainter and pass a pointer to the device.
    //A paint device can be a QWidget, a QPixmap or a QImage
    QPainter painter(this);


    // couple of variable to use when drawing
    int fromTop = 50;
    int offset = 30;
    int second2draw = 10;
    std::string label;
    QString qstr;
    int LeftMargin = 50;
    //float pixelFactor = 0.005;
    int scaleY = 50;


    // painter definition
    QPen signal_pen(Qt::black, 1, Qt::SolidLine,Qt::SquareCap, Qt::BevelJoin);
    QPen grid_pen(Qt::gray, 1, Qt::SolidLine,Qt::SquareCap, Qt::BevelJoin);

    // and some font
    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);


    for (int i = 1; i < second2draw; i++){
        painter.setPen(grid_pen);
        painter.drawLine(LeftMargin + i*250,0,LeftMargin + i*250,1000);
    }


    for (int i = 0; i < mainwindow->signal.recorder_info.numberOfChannelsUsed;i++){
        painter.setPen(grid_pen);
        painter.drawLine(LeftMargin,fromTop + offset*i,1000+LeftMargin,fromTop + offset*i);
        painter.setPen(signal_pen);
        label = mainwindow->signal.recorder_info.channels[i].channel_desc;
        qstr = QString::fromStdString(label);
        painter.drawText(1, fromTop+offset*i, qstr);
        int fromX = LeftMargin;
        int fromY = fromTop + offset*i;
        int timeWindow = mainwindow->signal.recorder_info.channels[i].sampling_rate*second2draw;

        for (int j = 0; j < timeWindow; j++){
            painter.drawLine(fromX+j, fromY-mainwindow->signal.signal_data[i][j]*scaleY, fromX+j+1, fromY-mainwindow->signal.signal_data[i][j+1]*scaleY);
        }
    }
}

ViewCurve::~ViewCurve()
{
    delete signal_pen;
    delete grid_pen;
}
