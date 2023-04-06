#include "ui_form.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <mainwindow.h>
#include <QStringList>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    this->setWindowTitle("setup");
    this->setFixedSize(211,190);
    serial->setBaudRate(ui->comboBox->currentIndex());
    QStringList baudlist;
    baudlist<<"9600";
    ui->comboBox->addItems(baudlist);

}

Form::~Form()
{
    delete ui;
}


