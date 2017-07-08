#include "mainwindow.h"

#include <QJsonObject>
#include <QDebug>
#include <QTimer>

#include "overviewwidget.h"
#include "server.h"
#include "controlthread.h"


//#define PRINT_BUG(var) qDebug() << __DATE__ << __TIME__ << objectName()+"::"+__func__ << #var+QString("=")+var << __FILE__ << __LINE__;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //setObjectName("MainWindow");
    OverviewWidget *w = new OverviewWidget;
    setCentralWidget(w);
    //if(w == 0)
        //PRINT_BUG(w)

    Server *server = &Server::initialization();
    connect(server, &Server::newControlThread, this, [=](ControlThread *thread){
        QTimer::singleShot(1000, [=]{
            w->add(thread->getJsonDataPtr(), "control");
            w->createTreeWidget();
        });
    });
}

MainWindow::~MainWindow()
{

}
