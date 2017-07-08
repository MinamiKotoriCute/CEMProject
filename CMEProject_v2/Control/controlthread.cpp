#include "controlthread.h"

#include <QDebug>

#include "control.h"

ControlThread::ControlThread(qintptr socketDescriptor, QObject *parent) :
    QThread(parent),
    socketDescriptor(socketDescriptor),
    createDateTime(QDateTime::currentDateTime())
{
    connect(this, &ControlThread::finished, this, &ControlThread::deleteLater);
}

void ControlThread::run()
{
    threadId = QThread::currentThreadId();
    control = new Control(socketDescriptor);
    connect(control, &Control::print, this, &ControlThread::print);
    connect(control, &Control::disconnected, this, &ControlThread::quit);
    QThread::run();
}

Control* ControlThread::getContorl()
{
    return control;
}

Qt::HANDLE ControlThread::getThreadId()
{
    return threadId;
}

QDateTime ControlThread::getCreateDateTime()
{
    return createDateTime;
}
