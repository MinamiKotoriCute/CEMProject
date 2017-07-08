#ifndef WEBCRAWLER_H
#define WEBCRAWLER_H

#include <QObject>

#include "webcrawler_global.h"

class WEBCRAWLERSHARED_EXPORT WebCrawler : public QObject
{
    Q_OBJECT
public:
    WebCrawler(QString username, QString password, QObject *parent = 0);

private:
    QString username, password;
};

#endif // WEBCRAWLER_H
