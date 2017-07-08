#include "control.h"

#include <QTcpSocket>
#include <QDebug>

#include "coursesearchanalyzer.h"
#include "database.h"
#include "expansion.h"
#include "collectthread.h"

Control::Control(QTcpSocket *socket, QObject *parent) :
    QObject(parent),
    socket(socket)
{
    connect(socket, &QTcpSocket::readyRead, this, &Control::socketReadyRead);

    connect(socket, &QTcpSocket::disconnected, this, &Control::socketDisconnected);
}

void Control::socketWrite(QByteArray b)
{
    if(socket)
        socket->write(b);
}

void Control::socketReadyRead()
{
    QByteArray receiveData = socket->readAll();
    QList<QByteArray> receiveDatas = receiveData.split(' ');
    if(receiveDatas.size() == 0){
        return;
    }

    emit print("active [" + receiveDatas.first() + "]");
    if(username.isEmpty() || password.isEmpty()){
        if(receiveDatas.first() == "login"){
            if(receiveDatas.size() == 3){
                CourseSearchAnalyzer courseSearchAnalyzer;
                courseSearchAnalyzer.home();
                if(courseSearchAnalyzer.login(receiveDatas.at(1), receiveDatas.at(2))){
                    username = receiveDatas.at(1);
                    password = receiveDatas.at(2);
                    Database db;
                    bool isFirstLogin = db.addNidAccount(username, password);
                    qDebug() << "isFirstLogin" << isFirstLogin;
                    if(isFirstLogin){
                        emit print(username + " CollectThread start");
                        CollectThread *thread = new CollectThread(username, password);
                        connect(thread, &CollectThread::finished, this, [=]{
                            emit print(username + " CollectThread finist!");
                            thread->deleteLater();
                        });
                        thread->start();
                    }
                    emit print("Login:" + username);
                    socketWrite("seccess");
                    return;
                }
            }
        }
    }
    else if(receiveDatas.first() == "electiveCourse"){
        if(receiveDatas.size() == 1){
            CourseSearchAnalyzer courseSearchAnalyzer;
            connect(&courseSearchAnalyzer, &CourseSearchAnalyzer::print, this, &Control::print);

            courseSearchAnalyzer.home();
            if(!username.isEmpty() && !password.isEmpty()){
                /*if(courseSearchAnalyzer.login(username, password)){

                }*/
            }
            QList<Course> strLL = courseSearchAnalyzer.electiveCoursesList("105", "1");
            QString sendMessage = "Q W E R";
            foreach (Course course, strLL) {
                qDebug() << course.courseName();
                sendMessage += course.electiveCode() + " " + course.courseName() + " "  + course.teacher() + " "  + QString::number((double)course.numberOfPeople().toInt()/course.totalOfPeople().toInt()) + '\n';
            }
            if(!strLL.isEmpty())
                sendMessage.remove(sendMessage.size()-1, 1);
            socketWrite(sendMessage.toLocal8Bit());
            emit print(sendMessage);
            return;
        }
    }
    else if(receiveDatas.first() == "popularity"){
        if(receiveDatas.size() == 2){
            QString electiveCode = receiveDatas.at(1);
            int code = electiveCode.toInt();
            if(code<10000){
                CourseSearchAnalyzer courseSearchAnalyzer;
                connect(&courseSearchAnalyzer, &CourseSearchAnalyzer::print, this, &Control::print);

                courseSearchAnalyzer.home();
                if(!username.isEmpty() && !password.isEmpty()){
                    //if(courseSearchAnalyzer.login(username, password))
                    //Course course = courseSearchAnalyzer.courseFromElectiveCode("105", "2", electiveCode);
                    double popularity = courseSearchAnalyzer.coursePopularity(electiveCode);
                    Course course = courseSearchAnalyzer.courseNameFromElectiveCode("105", "2", electiveCode);

                    QString sendMessage = course.courseName() + " " + course.teacher() + " " + QString::number(popularity) + "\n";
                    socketWrite(sendMessage.toLocal8Bit());
                    emit print(username + " " + sendMessage);
                    return;
                }
            }
        }
    }

    emit print(username + " fail");
    socket->write("fail");
}

void Control::socketDisconnected()
{
    deleteLater();
    socket->deleteLater();
    socket = 0;

    emit print("disconnected:" + username);
    emit disconnected();
}

Course Control::searchCourse(QString year, QString semester, QString courseName, QString electiveCode)
{
    Database db;
    Course course = db.courseInformation(electiveCode, year, semester);
    if(!course.isNull()){
        return course;
    }

    CourseSearchAnalyzer courseSearchAnalyzer;
    courseSearchAnalyzer.home();
    QList<Course> courseList = courseSearchAnalyzer.coursesList(year, semester, courseName, electiveCode);
    if(courseList.size() == 1)
        course = courseList.first();

    return course;
}

QList<Course> Control::electiveCoursesAnalysis()
{
    QList<Course> retValue;

    CourseSearchAnalyzer courseSearchAnalyzer;
    courseSearchAnalyzer.home();
    /*if(!courseSearchAnalyzer.login(username, password)){
        return retValue;
    }*/

    int year = 105;
    int semester = 1;

    QList<Course> courseStrLL = courseSearchAnalyzer.electiveCoursesList(QString::number(year), QString::number(semester));
    //everyone class of now
    foreach(Course course, courseStrLL){
        //calculate 8 semester
        int totalPeople = 0;
        int remainPeople = 0;
        for(int i=0;i<7;i++){
            QString yearStr = QString::number(year-(i+1)/2);
            QString semesterStr = QString::number(semester+(i%2));
            QList<Course> targetCourse = courseSearchAnalyzer.coursesList(yearStr, semesterStr, course.courseName(), QString::number(course.electiveCode()));
            foreach (Course t_course, targetCourse) {
                totalPeople += t_course.totalOfPeople().toInt();
                remainPeople += t_course.numberOfPeople().toInt();
            }
        }

        Course tem_course = course;
        tem_course.set_totalOfPeople(QString::number(totalPeople));
        tem_course.set_numberOfPeople(QString::number(remainPeople));
        retValue << tem_course;

        qDebug() << course.courseName() << course.teacher() << totalPeople << remainPeople;
    }
    //qDebug() << courseStrLL;
    return retValue;
}
