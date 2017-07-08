#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTimer>

#include "serverthread.h"
#include "control.h"
#include "defineheader.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ServerThread *t = &ServerThread::initialization();
    t->start();

    MainWindow w;
    w.show();

    qDebug() << QThread::currentThreadId();

    QTimer::singleShot(1000, [=]{
        Control *c = new Control;
        QTcpSocket *socket = new QTcpSocket;
        QObject::connect(socket, &QTcpSocket::readyRead, [=]{
            //c->getMessage(QString::fromUtf8(socket->readAll()));
            c->getMessage(QString(""));
            qDebug() << "aaaaaaaaaaaaaaaaaa";
        });
        socket->connectToHost("127.0.0.1", 55512);
        socket->waitForConnected();

        /*QTimer::singleShot(2000, [=]{
            socket->waitForReadyRead(100);
            qDebug() << QThread::currentThreadId() << socket->readAll();
        });*/
    });

    QTimer::singleShot(2000, [=]{
        QTcpSocket *socket = new QTcpSocket;
        socket->connectToHost("127.0.0.1", 55512);
        socket->waitForConnected();
        socket->write("QQQ");
        socket->waitForBytesWritten();
        //socket->waitForReadyRead(300);
        //qDebug() << QThread::currentThreadId() << socket->readAll();
        socket->disconnectFromHost();
        qDebug() << QThread::currentThreadId() << "end";
    });


    QObject::connect(&a, &QApplication::lastWindowClosed, t, &ServerThread::quit);

    return a.exec();
}
