#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();
    QSerialPort *serial;





    Ui::Form *ui;
};

#endif // FORM_H
