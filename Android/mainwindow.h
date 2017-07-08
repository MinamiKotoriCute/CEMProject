#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Network;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void setLoginWidget();
    void setElectiveCoursesWidget();
    void setPopularityWidget();
    void setLicenseWidget();

    Network *network;
};

#endif // MAINWINDOW_H
