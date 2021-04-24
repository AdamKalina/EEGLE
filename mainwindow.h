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
#include <QString>
#include <QFileDialog>
#include <QElapsedTimer>
#include <QDropEvent>
#include <QMimeData>
#include "read_signal_file.h"
#include "show_hdr.h"
#include "viewcurve.h"
//#include "third_party\fidlib-0.9.10\fidlib.h"

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

    //defaults - TO DO - class that stores default options?
    int testing = 0;
    int file_open = 0;
    long long pagetime = 5; // number of seconds to show on the screen
    long long viewtime = 0; //start of left edge of viewed page in seconds
    int mouseWheel; //1 = step, 0 = page
    string path2file;


    int foo;
    long long lengthOfFile;
    int notch = 0;

    // Methods

    void setFoo(int i){
        foo=i;
    }
    int getFoo(){
        return foo;
    }

    void test_patinfo(Measurement *measurement);
    void open_file(string path2file);
    void setup_viewbuf();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent* event);



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
    *helpmenu,
    *timemenu,
    *amplitudemenu,
    *montagemenu,
    *settingsmenu;

    QActionGroup *mousewheelgroup,
    *keyboardgroup,
    *fixedpagegroup,
    *fixedresolutiongroup,
    *timescalegroup;

    QAction *mousePageAction,
    *mouseStepAction,
    *BrainLabAction,
    *NicOneAction,
    *BLdefaultAction,
    *fiveCmPerSec,
    *twoAndHalfCmPerSec,
    *fiveSecPerPageAction,
    *tenSecPerPageAction,
    *fifteenSecPerPageAction,
    *twentySecPerPageAction,
    *thirtySecPerPageAction;

public slots:
    void show_about_dialog();
    void show_patient_info();
    void shift_page_left();
    void shift_page_right();
    void next_page();
    void previous_page();
    void first_page();
    void last_page();
    void open_file_dialog();
    void initialize();


private slots:
    void show_kb_shortcuts();

};
#endif // MAINWINDOW_H
