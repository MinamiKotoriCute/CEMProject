#include "control.h"

#include <QDebug>
#include <QThread>

#include "defineheader.h"

//#define PRINT_BUG(var) qDebug() << __DATE__ << __TIME__ << objectName()+"::"+__func__ << #var+QString("=")+var << __FILE__ << __LINE__;

Control::Control(QObject *parent) : QObject(parent)
{
    static int count = 0;
    setObjectName(QString::asprintf("Control %d", ++count));
    jsonData["Name"] = objectName();
    qDebug() << objectName();
}

void Control::getMessage(QString &msg)
{
    PRINT_BUG(msg)
    QStringList msgList = msg.split(" ");
    if(msgList.size() == 0){
        PRINT_BUG(msg)
        return;
    }

    QString msgHeader = msgList.first();
    emit sendToAllMessage("");
}

QJsonObject* Control::getJsonDataPtr()
{
    return &jsonData;
}
