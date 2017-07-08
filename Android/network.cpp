#include "network.h"

#include <QTcpSocket>
#include <QTextCodec>

#include "asynchronous.h"

Network::Network(QObject *parent) :
    QObject(parent), socket(new QTcpSocket(this))
{

}

bool Network::connectToHost()
{
    socket->connectToHost("127.0.0.1", 9527);
    //socket->connectToHost("220.133.245.47", 9527);
    return socket->waitForConnected();
}

bool Network::login(QString username, QString password)
{
    QString str = "login " + username + " " + password;
    QByteArray message = sendMessageWaitReturn(str.toLatin1());

    if(message == "seccess")
        return true;

    return false;
}

QList<QStringList> Network::electiveCourse()
{
    QByteArray message = sendMessageWaitReturn("electiveCourse");

    QTextCodec *codec = QTextCodec::codecForName("Big5");
    //QString str = QString::fromLocal8Bit(message);
    //QString str = QString::fromLatin1(message);
    //QString str = QString::fromUtf8(message);
    QString str = codec->toUnicode(message);
    //QStringList strList = str.split("\n");
    QList<QStringList> strLL;
    foreach (QString course, str.split("\n")) {
        if(course.isEmpty())
            continue;
        QStringList strList;
        foreach (QString courseData, course.split(" ")) {
            strList << courseData;
        }
        if(!strList.isEmpty())
            strLL << strList;
    }

    return strLL;
}

QStringList Network::coursePopularity(QString elective)
{
    QTextCodec *codec = QTextCodec::codecForName("Big5");

    QString sendMessage = "friendliness " + elective;
    qDebug() << sendMessage;
    QByteArray message = sendMessageWaitReturn(codec->fromUnicode(sendMessage));
    qDebug() << message;
    qDebug() << QString::fromLocal8Bit(message);
    qDebug() << QString::fromLatin1(message);
    qDebug() << QString::fromUtf8(message);
    qDebug() << codec->toUnicode(message);

    //QString str = QString::fromLocal8Bit(message);
    //QString str = QString::fromLatin1(message);
    //QString str = QString::fromUtf8(message);
    QString str = codec->toUnicode(message);
    //QStringList strList = str.split("\n");

    QStringList strList;
    foreach (QString course, str.split("\n")) {
        if(!course.isEmpty()){
            QStringList strList2 = course.split(" ");
            foreach (QString s, strList2) {
                strList << s;
            }
        }
    }
    qDebug() << strList;

    return strList;
}

QByteArray Network::sendMessageWaitReturn(QByteArray b)
{
    Asynchronous asynchronous;
    QByteArray message;
    QMetaObject::Connection con = connect(socket, &QTcpSocket::readyRead, this, [=, &message, &asynchronous]{
        if(!asynchronous.isRunning())
            return;

        message = socket->readAll();
        asynchronous.finish();
    });

    socket->write(b);
    asynchronous.start(30000);
    disconnect(con);

    return message;
}
