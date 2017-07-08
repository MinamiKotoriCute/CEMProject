#include "mainwindow.h"

#include <QDebug>
#include <QTextEdit>
#include <QTcpSocket>

#include "networkserver.h"
#include "control.h"
#include "coursesearchanalyzer.h"
#include "coursesearch.h"
#include "learningprotfolio.h"
#include "database.h"
#include "course.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    edit = new QTextEdit;
    setCentralWidget(edit);

    NetworkServer *s = NetworkServer::initialization();
    s->listen(9527);

    connect(s, &NetworkServer::newConnected, this, [=](QTcpSocket *socket){
        Control *c = new Control(socket);
        connect(c, &Control::print, this, &MainWindow::print);
        connect(c, &Control::disconnected, this, [=]{
            //print("diconnected");
        });
        print("\nnew connect");
    });

    //Database d;
    //LearningProtfolio lll;
    //CourseSearchAnalyzer csa;
    //csa.home();
    //qDebug() << csa.coursePopularity("2984");
    //Course course = csa.courseNameFromElectiveCode("105", "2", "1300");
    //csa.courseListFromCourse(course);
    //qDebug() << "login" << csa.login("D0250179", "joeyten2000123");
    //qDebug() << "login" << csa.login("D0450382", "asdqwezxc");
    /*qDebug() << csa.electiveCoursesList("105", "1");
    qDebug() << csa.electiveCoursesList("104", "2");*/
    //CourseSearchAnalyzer cccc;
    //cccc.home();
}

MainWindow::~MainWindow()
{

}

void MainWindow::print(QString str)
{
    qDebug() << str;
    edit->append(str);
}
