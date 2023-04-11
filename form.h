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
        void on_pushButton_2_clicked();
        QSerialPort *serial;
        int m;


signals:
    void buttonclicked();


private slots:
    void on_pushButton_clicked();

private:
    Ui::Form *ui;
};

#endif // FORM_H
