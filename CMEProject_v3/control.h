#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QJsonObject>

class ControlThread;

class Control : public QObject
{
    Q_OBJECT

public:
    explicit Control(QObject *parent = 0);
    void getMessage(QString &msg);
    QJsonObject* getJsonDataPtr();

signals:
    void sendMessage(const QString &);
    void sendToAllMessage(const QString &);
    void jsonDataChanged();

public slots:

private:
    QJsonObject jsonData;
};

#endif // CONTROL_H
