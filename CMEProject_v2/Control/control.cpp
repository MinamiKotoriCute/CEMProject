#include "control.h"

#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include <QTextCodec>

#include "collectthread.h"
#include "Database/database.h"
#include "WebCrawler/coursesearch.h"
#include "WebCrawler/learningprotfolio.h"
#include "WebCrawler/electivesystem.h"

Control::Control(qintptr socketDescriptor, QObject *parent)
    : QObject(parent),
      socket(new QTcpSocket(this)),
      codec(QTextCodec::codecForName("BIG-5")),
      courseSearch(new CourseSearch(this))
{
    emit print(QString::asprintf("%p Control::Control", QThread::currentThreadId()));
    connect(socket, &QTcpSocket::readyRead, this, &Control::getMessage);
    connect(socket, &QTcpSocket::disconnected, this, &Control::_disconnected);

    socket->setSocketDescriptor(socketDescriptor);
}

void Control::test()
{
    //qDebug() << QThread::currentThreadId() << "Control::test";
    //courseSearch->home();
    LearningProtfolio learningProtfolio;
    learningProtfolio.home();
    qDebug() << "learningProtfolio login" << learningProtfolio.login("D0352276", "Fk224891501");
    int n = 0;
    foreach (QJsonObject json, learningProtfolio.test()) {
        if(n%2 == 0){
            qDebug() << json["courseName"].toString();
        }
        else
            qDebug() << json["rank"].toString();
        n++;
    }

    /*CollectThread *thread = new CollectThread("D0250179", "joeyten2000123", 2);
    connect(thread, &CollectThread::print, this, &Control::print);
    thread->start();*/

    /*CourseSearch courseSearch;
    courseSearch.home();
    foreach (QJsonObject course, courseSearch.courseFromWeek("105", "2", "3")) {
        qDebug() << course;
    }*/
}

QString Control::getCourseSearchServerNumber()
{
    return courseSearch->getServerNumber();
}

QString Control::getUserName()
{
    return username;
}

void Control::getMessage()
{
    QString message = codec->toUnicode(socket->readAll());
    qDebug() << QThread::currentThreadId() << "read" << message;

    QStringList receiveDatas = message.split(' ');
    if(receiveDatas.size() == 0){
        return;
    }

    emit print("active [" + receiveDatas.first() + "]");
    if(username.isEmpty() || password.isEmpty()){
        if(receiveDatas.first() == "login"){
            if(receiveDatas.size() == 3){
                CourseSearch courseSearch;
                courseSearch.home();
                qDebug() << receiveDatas.at(1) << receiveDatas.at(2);
                if(courseSearch.login(receiveDatas.at(1), receiveDatas.at(2))){
                    username = receiveDatas.at(1);
                    password = receiveDatas.at(2);
                    Database db;
                    bool isFirstLogin = db.addNidAccount(username, password);
                    qDebug() << "isFirstLogin" << isFirstLogin;
                    if(isFirstLogin){
                        emit print(username + " CollectThread start");
                        CollectThread *thread = new CollectThread(username, password);
                        connect(thread, &CollectThread::finished, this, [=]{
                            qDebug() << username << "CollectThread finist!";
                            emit print(username + " CollectThread finist!");
                            thread->deleteLater();
                        });
                        thread->start();
                    }
                    emit print("Login:" + username);
                    socket->write("seccess");
                    return;
                }
            }
        }
    }
    else if(receiveDatas.first() == "friendliness"){
        if(receiveDatas.size() == 2){
            QString electiveCode = receiveDatas.at(1);
            int code = electiveCode.toInt();
            if(code<10000){
                Database db;
                QJsonObject course;
                course["year"] = "105";
                course["semester"] = "2";
                course["electiveCode"] = QString::number(code);

                QJsonObject courseInfo = db.getCourseAnalysis(course);
                if(!courseInfo["courseName"].toString().isEmpty()){
                    QString sendMessage = courseInfo["courseName"].toString() + " " + courseInfo["teacher"].toString() + " " + courseInfo["friendliness"].toString() + "\n";
                    socket->write(codec->fromUnicode(sendMessage));
                    qDebug() << username << sendMessage;
                    emit print(username + " " + sendMessage);
                    return;
                }
            }
        }
    }

    emit print(username + " fail");
    socket->write("fail");
}

void Control::_disconnected()
{
    emit disconnected();
}
