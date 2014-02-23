#include <QtCore/QDebug>

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

    ui->buttonRX->click();
    ui->buttonTX->click();
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
    if (rxenabled)
        qDebug() << data;
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
