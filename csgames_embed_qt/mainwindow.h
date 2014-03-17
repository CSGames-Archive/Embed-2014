#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/qserialport.h>
#include <QtCore/QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSerialPort serial;
    
private:
    Ui::MainWindow *ui;
    bool rxenabled;
    bool txenabled;

public slots:
    void readData();
    void toggleRX();
    void toggleTX();
    void textChanged();
    void ledIntensityChanged();
    void timerTick();
};

#endif // MAINWINDOW_H
