#include "mainwindow.h"

#include <QMessageBox>
#include <QLabel>
#include <QDebug>
#include <QFile>

#include "loginwidget.h"
#include "network.h"
#include "electivecourseswidget.h"
#include "popularitywidget.h"
#include "licensewidget.h"

#define TEMFILENAME "info"

#define TEMVERSION 1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    network(new Network)

{
    if(!network->connectToHost()){
        QMessageBox::about(this, QStringLiteral("Oops"), QStringLiteral("伺服器維修中"));
        exit(0);
    }

    QFile file(TEMFILENAME);
    if(file.open(QFile::ReadOnly)){
        QByteArray b = file.readAll();
        int version = b.toInt();
        if(version >= TEMVERSION){
            setLoginWidget();
            return;
        }
        file.close();
    }

    setLicenseWidget();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setLoginWidget()
{
    LoginWidget *loginWidget = new LoginWidget;
    setCentralWidget(loginWidget);

    connect(loginWidget, &LoginWidget::login, this, [=](QString username, QString password){
        if(network->login(username, password)){
            //QMessageBox::information(this, "from server", "seccess");
            //setElectiveCoursesWidget();
            setPopularityWidget();
        }
        else{
            loginWidget->reset();
            QMessageBox::information(this, "from server", "fail");
        }
    });
}

void MainWindow::setElectiveCoursesWidget()
{
    ElectiveCoursesWidget *electiveCoursesWidget = new ElectiveCoursesWidget(network->electiveCourse());
    setCentralWidget(electiveCoursesWidget);

    connect(electiveCoursesWidget, &ElectiveCoursesWidget::logout, this, [=]{
        setLoginWidget();
    });
}

void MainWindow::setPopularityWidget()
{
    PopularityWidget *popularityWidget = new PopularityWidget;
    setCentralWidget(popularityWidget);

    connect(popularityWidget, &PopularityWidget::send, this, [=](QString sendMessage){
        QStringList strList = network->coursePopularity(sendMessage);
        popularityWidget->get(strList);
    });
}

void MainWindow::setLicenseWidget()
{
    LicenseWidget *licenseWidget = new LicenseWidget;
    setCentralWidget(licenseWidget);

    connect(licenseWidget, &LicenseWidget::ok, this, [=]{
        QFile file(TEMFILENAME);
        if(file.open(QFile::WriteOnly)){
            file.write(QByteArray::number(TEMVERSION));
            file.close();
        }

        setLoginWidget();
    });
}
