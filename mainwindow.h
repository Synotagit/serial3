#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSerialPort *serial;
    void UpdatePort();
    QStringList oldPortStringList;
    QTimer *timer;

 unsigned int  crc32( const unsigned char *buf, unsigned int size);

private slots:
    void on_openserialbtn_clicked();



    void on_sendbtn_clicked();

    void on_moreset_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort SerialPort;

};
#endif // MAINWINDOW_H
