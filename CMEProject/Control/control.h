#ifndef CONTROL_H
#define CONTROL_H

#include "control_global.h"

#include <QObject>

//#include "course.h"

#include "../course/course.h"

class QTcpSocket;

class CONTROLSHARED_EXPORT Control : public QObject
{
    Q_OBJECT
public:
    Control(QTcpSocket *socket, QObject *parent = 0);

signals:
    void print(QString);
    void disconnected();

private slots:
    void socketWrite(QByteArray b);

    void socketReadyRead();
    void socketDisconnected();

private:
    Course searchCourse(QString year, QString semester, QString courseName, QString electiveCode);
    QList<Course> electiveCoursesAnalysis();

    QTcpSocket *socket;
    QString username, password;
};

#endif // CONTROL_H
