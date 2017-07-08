#include "mainwindow.h"

#include <QThread>
#include <QTimer>
#include <QTabWidget>

#include "overviewwidget.h"
#include "iowidget.h"

#include "Control/control.h"
#include "Control/controlthread.h"
#include "NetworkServer/networkserver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << QThread::currentThreadId() << "MainWindow::MainWindow";
    NetworkServer *networkServer = new NetworkServer(this);
    networkServer->listen(QHostAddress::Any, 9527);

    OverviewWidget *overviewWidget = new OverviewWidget(networkServer->getControlThreads());
    IOWidget *iowidget = new IOWidget;
    connect(networkServer, &NetworkServer::newConnected, this, [=](ControlThread *thread){
        iowidget->print("new connected");
        connect(thread, &ControlThread::print, iowidget, &IOWidget::print);
    });

    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->addTab(overviewWidget, "Overview");
    tabWidget->addTab(iowidget, "IO");
    setCentralWidget(tabWidget);

    // *************** test ***************.
    if(networkServer->getControlThreads().size() != 0){
        Control *control = networkServer->getControlThreads().first()->getContorl();
        QTimer::singleShot(10, control, &Control::test);
    }

}

MainWindow::~MainWindow()
{

}
