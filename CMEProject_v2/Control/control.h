#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>

class QTcpSocket;
class CourseSearch;

class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(qintptr socketDescriptor, QObject *parent = 0);

    void test();

    QString getCourseSearchServerNumber();
    QString getUserName();

signals:
    void print(QString);
    void disconnected();

public slots:

private slots:
    void getMessage();
    void _disconnected();

private:
    QString lastMessage;
    QString username, password;

    CourseSearch *courseSearch;
    QTextCodec *codec;
    QTcpSocket *socket;
};

#endif // CONTROL_H
