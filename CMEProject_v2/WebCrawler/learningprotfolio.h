#ifndef LEARNINGPROTFOLIO_H
#define LEARNINGPROTFOLIO_H

#include <QObject>
#include <QJsonObject>

class WebAutomaticIndexer;

class LearningProtfolio : public QObject
{
    Q_OBJECT
public:
    explicit LearningProtfolio(QObject *parent = 0);

    void home();
    bool login(QString username, QString password);
    QList<QJsonObject> test(int n = -1);

    QString getLastStatus();

signals:

public slots:

private:
    QList<QJsonObject> courseTranscript(QByteArray url);
    QByteArray myTranscripts();
    QByteArray myTranscripts(int n);
    QStringList findLearningProtfolioValue(const QString &source, QMap<QString, QString> *in_out_map = 0);
    QStringList findLearningProtfolioValue2(QByteArray lastHtml, int n);

    QString lastStatus;
    int maxSemester;
    QByteArray lastHtml;
    QTextCodec *codec;
    WebAutomaticIndexer *web;
    QString lastYear, lastSemester;
    QString studentID;
};

#endif // LEARNINGPROTFOLIO_H
