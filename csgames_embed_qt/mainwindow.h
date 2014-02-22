#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/qserialport.h>

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

public slots:
    void toggleRX();
    void toggleTX();
};

#endif // MAINWINDOW_H
