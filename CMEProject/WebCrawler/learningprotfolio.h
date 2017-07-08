#ifndef LEARNINGPROTFOLIO_H
#define LEARNINGPROTFOLIO_H

#include <QObject>

#include "webcrawler_global.h"

class WebAutomaticIndexer;

class WEBCRAWLERSHARED_EXPORT LearningProtfolio : public QObject
{
    Q_OBJECT
public:
    explicit LearningProtfolio(QObject *parent = 0);

    void home();
    bool login(QString username, QString password);
    void test(int n = -1);

signals:

public slots:

private:
    QStringList courseTranscript(QByteArray url);
    QByteArray myTranscripts();
    QByteArray myTranscripts(int n);

    QStringList findLearningProtfolioValue2(QByteArray lastHtml, int n);
    QStringList findLearningProtfolioValue(const QString &source, QMap<QString, QString> *in_out_map = 0);

    int maxSemester;
    QByteArray lastHtml;
    QTextCodec *codec;
    WebAutomaticIndexer *web;
    QString lastYear, lastSemester;
};

#endif // LEARNINGPROTFOLIO_H
