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
#include <QToolBar>
#include <QDoubleSpinBox>
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
    struct read_signal_file::SignalFile signal;
    struct read_signal_file::SignalFile SF;
    struct read_signal_file::Measurement HDR;
    struct read_signal_file::SignalFile *pSF;

    // Variables

    //defaults - TO DO - class that stores default options?
    int testing = 0;
    int file_open = 0;
    long long pagetime = 10; // number of seconds to show on the screen
    long long viewtime = 0; //start of left edge of viewed page in seconds
    int mouseWheel = 0; //1 = step, 0 = page
    std::string path2file;


    int foo;
    long long lengthOfFile;
    int notch = 0; // notch filter on/off

    // Methods

    void setFoo(int i){
        foo=i;
    }
    int getFoo(){
        return foo;
    }

    void test_patinfo(read_signal_file::Measurement *measurement);
    void open_file(std::string path2file);
    void setup_viewbuf();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent* event);
    void createToolbars();


    void setHDR(read_signal_file::Measurement measurement){
        HDR = measurement;
    }

    read_signal_file::Measurement getHDR(){
        return HDR;
    }

private:
    QMenuBar     *menubar;
    QToolBar *toolBar;

    QMenu   *filemenu,
    *infomenu,
    *helpmenu,
    *timemenu,
    *amplitudemenu,
    *montagemenu,
    *settingsmenu,
    *notchmenu;

    QActionGroup *mousewheelgroup,
    *keyboardgroup,
    *fixedpagegroup,
    *fixedresolutiongroup,
    *timescalegroup,
    *notchgroup;

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
    *thirtySecPerPageAction,
    *notchOnAction,
    *notchOffAction,
    *notchButton,
    *LFF;

    QLabel *LFFLabel,
    *HFFLabel;

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
    void mousewheel_mode_page();
    void mousewheel_mode_step();
    void notchToggle(bool checked);


private slots:
    void show_kb_shortcuts();

};
#endif // MAINWINDOW_H
