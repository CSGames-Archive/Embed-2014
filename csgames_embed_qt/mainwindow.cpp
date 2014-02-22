#include <QtCore/QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::toggleRX()
{
    qDebug() << "RX toggled";
}

void MainWindow::toggleTX()
{
    qDebug() << "TX toggled";
}
