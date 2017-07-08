#include "controlthread.h"

#include <QTcpSocket>
#include <QTextCodec>
#include <QTimer>

#include "control.h"

ControlThread::ControlThread(qintptr socketDescriptor, QObject *parent) :
    QThread(parent),
    socketDescriptor(socketDescriptor),
    codec(QTextCodec::codecForName("UTF-8"))
{
    //while thread finished, delete it.
    connect(this, &ControlThread::finished, this, &ControlThread::deleteLater);
}

QJsonObject* ControlThread::getJsonDataPtr()
{
    return control->getJsonDataPtr();
}

void ControlThread::run()
{
    control = new Control;
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);

    //when socket get message, transform to string and send to control.
    connect(socket, &QTcpSocket::readyRead, [=]{
        QByteArray binaryArray = socket->readAll();
        QString string = codec->toUnicode(binaryArray);
        control->getMessage(string);
    });

    //when get sendMessage signal, transform to binary array and send by socket.
    connect(this, &ControlThread::sendMessage, socket, [=](const QString &msg){
        QByteArray binaryArray = codec->fromUnicode(msg);
        socket->write(binaryArray);
    });

    connect(control, &Control::sendMessage, this, &ControlThread::sendMessage);
    connect(control, &Control::sendToAllMessage, this, &ControlThread::sendToAllMessage);

    //when socket disconnected, free memory and exit this thread.
    connect(socket, &QTcpSocket::disconnected, [=]{
        control->deleteLater();
        socket->deleteLater();
        exit(0);
    });

    QTimer::singleShot(0, this, [=]{
        emit enterEventLoop();
    });

    //start event loop
    exec();
}
