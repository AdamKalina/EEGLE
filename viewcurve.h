#ifndef VIEWCURVE_H
#define VIEWCURVE_H
#include <QPainter>
#include <QPen>
#include <QStaticText>
#include <QShortcut>
#include <QMouseEvent>
#include <QScreen>
#include "third_party\fidlib\fidlib.h"
#include "mainwindow.h"
#include <stdio.h>
#include <string.h>
#include <cstring>


class MainWindow;

class ViewCurve : public QWidget
{
    Q_OBJECT

public:
    ViewCurve(QWidget *parent=0);
    ~ViewCurve();

    //void drawCurve_stage_1(QPainter *painter=NULL);
    MainWindow *mainwindow;

    //SignalFile SF;

    //fidlib
    FidFilter *ff;
    FidRun *run;
    FidFunc *funcp;
    void *fbuf1, *fbuf2;

    QShortcut *shift_page_left_shortcut,
    *shift_page_right_shortcut,
    *next_page_shortcut,
    *previous_page_shortcut,
    *first_page_shortcut,
    *last_page_shortcut;

    // metody

    void arrowkeys_shortcuts_global_set_enabled(bool);


protected:
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *);


private:
    QPen *signal_pen,
    *grid_pen;
    int fromTop, offset, second2draw, LeftMargin, scaleY;
    std::string label;
    QString qstr;
    float pixelFactor;
    QFont font;
};

#endif // VIEWCURVE_H
