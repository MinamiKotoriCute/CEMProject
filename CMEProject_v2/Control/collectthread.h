#ifndef COLLECTTHREAD_H
#define COLLECTTHREAD_H

#include <QThread>

class CollectThread : public QThread
{
    Q_OBJECT
public:
    explicit CollectThread(QString username, QString password, int status = 0, QObject *parent = 0);

signals:
    void print(QString);

public slots:

protected:
    void run();

private:
    QString username, password;
    int status;
};

#endif // COLLECTTHREAD_H
