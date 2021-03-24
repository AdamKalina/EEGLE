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

    // Structs

    //void paintEvent(QPaintEvent *event);
    struct SignalFile signal;
    struct SignalFile SF;
    struct Measurement HDR;
    struct SignalFile *pSF;

    // Variables

    int foo;
    int files_open;
    int mouseWheel;
    long long pagetime;
    long long lengthOfFile;
    long long viewtime;

    // Methods
    int getFoo(){
        return foo;
    }

    void test_patinfo(Measurement *measurement);
    void setup_viewbuf();

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
    *helpmenu;

public slots:
    void show_about_dialog();
    void show_patient_info();
    void shift_page_left();
    void shift_page_right();
    void next_page();
    void previous_page();
    void first_page();
    void last_page();


private slots:
    void show_kb_shortcuts();

};
#endif // MAINWINDOW_H
