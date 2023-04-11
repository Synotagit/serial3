#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QString>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#include <QDebug>
#include <form.h>
#include <QTimer>
#include <QtCore>
#include <QtGui>
#include <QFileDialog>
#include <QByteArray>
#include <QFile>
#include <QWidgetData>
#include <QTextCodec>
#include <QtMath>
#include <QTranslator>
#include <QDateTime>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //主窗口参数设置
    this->setWindowTitle("sscom");
    this->setFixedSize(900,600);
    dlgTitle = new QDialog(this);


    //新窗口参数设置
    this->nwindow = new Form();
    this->nwindow->setWindowTitle("setup");
    this->nwindow->setFixedSize(211,190);
    ui->sendedit->setFixedSize(700,100);

    //串口设置槽函数
    connect(nwindow,&Form::buttonclicked,this,&MainWindow::treat);




    //按键大小设置
    ui->openserialbtn->setFixedSize(60,100);
    ui->moreset->setFixedSize(100,30);

    //初始化串口
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        qDebug()<<info.portName()<<info.description();
        ui->portbox->addItem(info.portName());

        QSerialPort serial(info);
        QStringList baudlist;
        QStringList paritylist;
        QStringList databitslist;
        QStringList stopbitslist;

        baudlist<<"1200"<<"2400"<<"4800"<<"9600"<<"14400"<<
                  "19200"<<"38400"<<"56000"<<"57600"<<"115200";
//        ui->choosebit->addItems(baudlist);
//        ui->choosebit->setCurrentIndex(3);



        paritylist<<"无"<<"奇"<<"偶";




        //测试串口是否空闲
        if(serial.open(QIODevice::ReadWrite)){
            ui->portbox->addItem(info.portName());
            serial.close();
        }
        else{
            ui->portbox->addItem(info.portName()+"(被占用)");
        }
    }

    //未打开串口不使能发送按钮
    if(ui->openserialbtn->text()=="打开串口")
        ui->sendbtn->setEnabled(false);

    //1秒更新一次串口
    timer = new QTimer;
    connect(timer,&QTimer::timeout,this,&MainWindow::UpdatePort);
    timer->start(1000);

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::treat(){

}

//打开串口按钮槽
void MainWindow::on_openserialbtn_clicked()
{

    //如果串口为关闭状态时按下
    if(ui->openserialbtn->text()=="打开串口")
    {
        //串口为当前串口box显示的串口
        serial = new QSerialPort(ui->portbox->currentText());

        //使能发送按钮
        ui->sendbtn->setEnabled(true);

        //不使能portbox
        ui->portbox->setEnabled(false);

        //如果串口正在使用
        if(!serial->open(QIODevice::ReadWrite)){
            QMessageBox::warning(this,"tips","串口打开失败");
            serial->deleteLater();
            return;
        }

        //初始化
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setParity(QSerialPort::NoParity);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        //不使能portbox
        ui->portbox->setEnabled(false);

        //槽，使接收框接收数据
        connect(serial,&QSerialPort::readyRead,this,[&](){
            QByteArray arr = serial ->readAll();
            if(!arr.isEmpty())
                ui->receiveedit->insertPlainText(arr);

        });

        ui->openserialbtn->setText("关闭串口");
    }

    //串口处于打开状态
    else{
        //释放串口
        serial->clear();
        serial->close();
        serial->deleteLater();

        //使能portbox
        ui->portbox->setEnabled(true);
        //不使能发送按钮
        ui->sendbtn->setEnabled(false);
        ui->openserialbtn->setText("打开串口");
    }
}

//更新串口函数定义
void MainWindow::UpdatePort()
{
    //可使用的串口赋值给newPortStringList
    QStringList newPortStringList;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        newPortStringList += info.portName();
    }

    //把新串口加入portbox
    if(newPortStringList.size() != oldPortStringList.size())
    {
        oldPortStringList = newPortStringList;
        ui->portbox->clear();
        ui->portbox->addItems(oldPortStringList);
    }

}

//发送按钮槽()

void MainWindow::on_sendbtn_clicked()
{
    QByteArray arr = ui->sendedit->toPlainText().toLocal8Bit();
    //发送
    serial->write(arr);
}



//更多串口设置
void MainWindow::on_moreset_clicked()
{
    nwindow->show();
}
//清除窗口文本
void MainWindow::on_clearwin_clicked()
{
    ui->receiveedit->clear();
}



//打开文件
void MainWindow::on_openfile_clicked()
{
    QString curPath = QDir::currentPath();
    QString diltitle = "open file";
    QString filter = "文本文件(*.txt);;二进制文件(*.bin *.dat);;所有文件;(*.*);";
    QString filepath = QFileDialog::getOpenFileName(this,diltitle,curPath,filter);
    QFileInfo fileinfo(filepath);

    if(filepath.isEmpty()){
        QMessageBox::warning(this,"warning","file is empty");
        return;
    }

    ui->file->clear();
    ui->file->setText(filepath);

    QFile file(filepath);

    if(!file.exists())
    {
        QMessageBox::warning(this,"warning","file is not exist");
        return;
    }
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,"warining","file onlyready");
        return;
    }
    QByteArray data = file.readAll();

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QTextCodec::ConverterState state;
    QString fileText = QTextCodec::codecForName("UTF-8")->toUnicode(data.constData(),data.size(),&state);

    if(state.invalidChars>0)
    {
        fileText = QTextCodec::codecForName("GBK")->toUnicode(data);
    }
    else{
        fileText = QString(data);
    }


    int type = 0;
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(filepath);
    if(mime.name().startsWith("text/"))
    {
        type = 1;

    }
    else if (mime.name().startsWith("application/"))
    {
        type = 2;
    }
    switch(type){
    case 1:
    {

        file.close();
        if(data.isEmpty()){
            QMessageBox::information(this,"warning","file is empty");
            return;
        }
    }
        break;
    case 2:
    {
        int file2 = fileinfo.size();
        QVector<char> cBuf(file2);
        QDataStream datain(&file);
        datain.readRawData(&cBuf[0],file2);
        file.close();
        fileText = QString::fromLocal8Bit(&cBuf[0],file2);
    }
    }
    QString info = QString("%1%2").arg("file size is:").arg(data.length());
    ui->receiveedit->clear();
    ui->receiveedit->append(info);

    if(ui->hexshow->isChecked())
        ui->receiveedit->setPlainText(fileText.toUtf8().toHex(' ').toUpper());
    else
        ui->receiveedit->setPlainText(data);
    ui->receiveedit->moveCursor(QTextCursor::Start,QTextCursor::MoveAnchor);
    ui->progressBar->setValue(0);
    issendfile = true;
    progressbarvalue = 0;

}



//发送文件
void MainWindow::on_sendfile_clicked()
{


}

void MainWindow::File_TimerSend(void){

}

//窗口置顶勾选框槽函数
void MainWindow::on_zuiqian_stateChanged(int arg1)
{
        if (arg1 == 0)
        {
            ::SetWindowPos((HWND)this->winId(), HWND_NOTOPMOST, this->pos().x(), this->pos().y(), width(), height(), SWP_SHOWWINDOW);
        }
        else if (arg1 == 2)
        {
            ::SetWindowPos((HWND)this->winId(), HWND_TOPMOST, this->pos().x(), this->pos().y(), width(), height(), SWP_SHOWWINDOW);
        }
}

void MainWindow::sendfile(){
}

    //checkbox选择是否英文
void MainWindow::on_english_stateChanged(int arg1)
{
    if(arg1==2){
        tran.load(":/qt_cn.qm");
    }else if(arg1==0){
        tran.load(":/qt_en.qm");
    }
    //发送event信号
    qapp->installTranslator(&tran);
}
    //信号处理函数
void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}




