#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>

class QTcpSocket;

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = 0);

    bool connectToHost();
    bool login(QString username, QString password);
    QList<QStringList> electiveCourse();
    QStringList coursePopularity(QString elective);

signals:

public slots:

private:
    QByteArray sendMessageWaitReturn(QByteArray b);

    QTcpSocket *socket;
};

#endif // NETWORK_H
