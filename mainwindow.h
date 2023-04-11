#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <form.h>
#include <QFile>
#include <QTextCodec>
#include <QtMath>
#include <QTranslator>
#include <QApplication>
#include <QDateTime>
#include <QAction>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MainWindow(bool topmost,QWidget *parent=Q_NULLPTR);
    QSerialPort *serial;
    void UpdatePort();
    QStringList oldPortStringList;
    QTimer *timer;
    Form *nwindow;
    QDialog *dlgTitle;
    QFile *fileText;
    QString framelen;
    QString framenumber;
    QString lastframelen;
    QTimer *filesentimer;
    QString framecount;
    int progressbarvalue;
    int progressbarstep;
    bool issendfile ;
    void sendfile();
    void File_TimerSend(void);
    unsigned int  crc32( const unsigned char *buf, unsigned int size);
    void insertdatatoplain();
    QDateTime curdatetime;
    QByteArray uart_rec_ss;
    QString senduartdata(QString da,bool ishex,bool hastimestamp,bool isat);
    int send_buf_len;
    void serial_Read();
    void readToHex();
    void StringToHex(QString str, QByteArray &senddata);
    char convertHexChart(char ch);


private slots:
    void on_openserialbtn_clicked();

    void on_moreset_clicked();
    void treat();
    void on_clearwin_clicked();
    void on_openfile_clicked();
    void on_sendfile_clicked();
    void on_zuiqian_stateChanged(int arg1);
    void on_english_stateChanged(int arg1);
    void changeEvent(QEvent *e);


    void on_sendbtn_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort SerialPort;
    QTranslator tran;
    QApplication *qapp;


};
#endif // MAINWINDOW_H
