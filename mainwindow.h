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
#include "eeg_data_manager.h"
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
    read_signal_file::SignalFile signal;
    EegDataManager* m_dataManager = nullptr;

    // Variables
    //defaults - TO DO - class that stores default options?
    int testing = 0;
    int file_open = 0;
    long long pagetime = 10; // default number of seconds to show on the screen
    long long viewtime = 0; //start of left edge of viewed page in seconds
    int mouseWheelMode = 0; //1 = step, 0 = page
    std::string path2file;

    long long lengthOfFile;
    int notch = 0; // notch filter on/off

    // Methods
    void test_patinfo(read_signal_file::Measurement *measurement);
    void open_file(std::string path2file);
    void setup_viewbuf();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent* event);
    void createToolbars();

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
