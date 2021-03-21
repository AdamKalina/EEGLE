#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QLabel>
#include "read_signal_file.h"
#include "show_hdr.h"
#include "viewcurve.h"

class ViewCurve;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    ViewCurve    *maincurve;
    void test_patinfo(Measurement *measurement);
    //void paintEvent(QPaintEvent *event);
    struct SignalFile signal;
    struct SignalFile SF;
    struct Measurement HDR;
    struct SignalFile *pSF;
    int foo;
    int getFoo(){
        return foo;
    }

    void setFoo(int i){
        foo=i;
    }

    void setHDR(Measurement measurement){
        HDR = measurement;
    }

    Measurement getHDR(){
        return HDR;
    }

    void setSignal(SignalFile signal){
        SF = signal;
    }

    SignalFile getSignal(){
        return SF;
    }

private:
    QMenuBar     *menubar;

    QMenu   *filemenu,
            *infomenu,
            *aboutmenu;

public slots:
    void show_about_dialog();
    void show_patient_info();
};
#endif // MAINWINDOW_H
