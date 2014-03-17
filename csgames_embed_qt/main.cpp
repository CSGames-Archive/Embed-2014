#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString portName("ttyUSB0");
    bool found = false;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        //qDebug() << "Name        : " << info.portName();
        if (info.portName().startsWith(portName)) {
            found = 1;
            w.serial.setPort(info);
            if (!w.serial.open(QIODevice::ReadWrite)) {
                qDebug() << "ERROR: cant connect to " << info.portName();
                exit(1);
            }
            w.serial.setBaudRate(QSerialPort::Baud57600);
        }
    }
    if (!found) {
        portName = "ttyS0";
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            //qDebug() << "Name        : " << info.portName();
            if (info.portName().startsWith(portName)) {
                found = 1;
                w.serial.setPort(info);
                if (!w.serial.open(QIODevice::ReadWrite)) {
                    qDebug() << "ERROR: cant connect to " << info.portName();
                   // exit(1);
                }
                w.serial.setBaudRate(QSerialPort::Baud57600);
            }
        }
        if (!found) {
            qDebug() << "ERROR: cant find port " << portName;
          //  exit(1);
        }
    }

    w.show();
    
    return a.exec();
}
