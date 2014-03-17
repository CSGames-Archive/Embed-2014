#include <QtCore/QDebug>

#include <stdint.h>
#include <unistd.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    rxenabled = false;
    txenabled = false;

    QObject::connect(&serial, SIGNAL(readyRead()), this, SLOT(readData()));
    QObject::connect(ui->buttonTX, SIGNAL(clicked()), this, SLOT(toggleTX()));
    QObject::connect(ui->buttonRX, SIGNAL(clicked()), this, SLOT(toggleRX()));
    QObject::connect(ui->textSet, SIGNAL(clicked()), this, SLOT(textChanged()));
    QObject::connect(ui->textValue, SIGNAL(returnPressed()), this, SLOT(textChanged()));
    QObject::connect(ui->greenValue, SIGNAL(valueChanged(int)), this, SLOT(ledIntensityChanged()));
    QObject::connect(ui->redValue, SIGNAL(valueChanged(int)), this, SLOT(ledIntensityChanged()));
//    QObject::connect(ui->greenValue, SIGNAL(sliderMoved(int)), this, SLOT(ledIntensityChanged()));
//    QObject::connect(ui->redValue, SIGNAL(sliderMoved(int)), this, SLOT(ledIntensityChanged()));

    ui->greenValue->setMinimum(0);
    ui->greenValue->setMaximum(100);
    ui->redValue->setMinimum(0);
    ui->redValue->setMaximum(100);

    ui->buttonRX->click();
    ui->buttonTX->click();

    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    if (serial.isOpen())
        serial.close();
    delete ui;
}

void MainWindow::readData()
{
    QByteArray data = serial.readAll();

    if (!rxenabled)
        return;

    for (int i = 0; i < data.size(); i++)
    {
        uint8_t c = data[i];
        printf("%c", c);
        fflush(0);
        //qDebug() << (char)c;
    }
}

void MainWindow::toggleRX()
{
    rxenabled = ui->buttonRX->isChecked();
    qDebug() << "RX toggled: " << rxenabled;
}

void MainWindow::toggleTX()
{
    txenabled = ui->buttonTX->isChecked();
    qDebug() << "TX toggled: " << txenabled;
}

void MainWindow::textChanged()
{
    QString text = ui->textValue->text();
    qDebug() << "Text value: " << text;

    if (txenabled) {
        const char * data = text.toStdString().c_str();
        int len = text.size();
        for (int i = 0; i < len; i++) {
            serial.write(data+i, 1);
            serial.flush();
            usleep(2000); // 2 ms
        }
    }
}

void MainWindow::ledIntensityChanged()
{
    int red = ui->redValue->value();
    int green = ui->greenValue->value();
    qDebug() << "LED red " << red << " green " << green;
}

void MainWindow::timerTick()
{
    qDebug() << "Tick !";
    if (!serial.isOpen())
        return;

    QString text = "tick";
    const char * data = text.toStdString().c_str();
    int len = text.size();
    for (int i = 0; i < len; i++) {
        serial.write(data+i, 1);
        serial.flush();
        usleep(2000); // 2 ms
    }
}
