#ifndef WEBCRAWLER_H
#define WEBCRAWLER_H

#include <QObject>

class WebCrawler : public QObject
{
    Q_OBJECT
public:
    explicit WebCrawler(QObject *parent = 0);

signals:

public slots:
};

#endif // WEBCRAWLER_H
