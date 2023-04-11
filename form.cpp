#include "form.h"
#include "ui_form.h"
#include <mainwindow.h>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    QStringList baud;
    baud<<"9600";
    ui->comboBox->addItems(baud);
//    m=ui->comboBox->currentIndex();
//    serial->setBaudRate(m);

}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_2_clicked()
{
    close();
}



void Form::on_pushButton_clicked()
{
      emit buttonclicked();
}
