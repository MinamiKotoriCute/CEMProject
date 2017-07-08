#ifndef ELECTIVESYSTEM_H
#define ELECTIVESYSTEM_H

#include <QObject>

class WebAutomaticIndexer;

class ElectiveSystem : public QObject
{
    Q_OBJECT
public:
    explicit ElectiveSystem(QObject *parent = 0);

    bool home();
    bool login(QString username, QString password);
    void search(QString electiveCode);

signals:

public slots:

private:
    QStringList findPostDataValue(const QString &source);

    QString mainUrl;
    QByteArray firstHtml;
    QString checkCode;
    QTextCodec *codec;
    WebAutomaticIndexer *web;
};

#endif // ELECTIVESYSTEM_H
