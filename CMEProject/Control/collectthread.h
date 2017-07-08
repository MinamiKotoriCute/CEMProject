#ifndef COLLECTTHREAD_H
#define COLLECTTHREAD_H

#include <QObject>
#include <QThread>

class CollectThread : public QThread
{
    Q_OBJECT
public:
    explicit CollectThread(QString username, QString password);

signals:

public slots:

protected:
    void run();

private:
    QString username, password;

    int status;
};

#endif // COLLECTTHREAD_H
