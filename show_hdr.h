#ifndef SHOW_HDR_H
#define SHOW_HDR_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QTextEdit>
#include <QTextCodec>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QDebug>
#include <QFont>
#include <QApplication>
#include <QDateTime>
#include <QWidget>
#include "mainwindow.h"

class MainWindow;

class hdrwindow : public QObject
{
  Q_OBJECT

public:
  hdrwindow(QWidget *parent);
  MainWindow *mainwindow;

private:

QDialog      *AboutDialog;

QPushButton  *pushButton1;

QTextEdit    *textedit1;

QHBoxLayout  *hlayout1;

QVBoxLayout  *vlayout1;

QFont *myfont;

//Measurement patinfo;


};
#endif // SHOW_HDR_H
