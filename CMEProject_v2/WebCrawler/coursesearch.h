#ifndef COURSESEARCH_H
#define COURSESEARCH_H

#include <QObject>
#include <QJsonObject>

class WebAutomaticIndexer;

class CourseSearch : public QObject
{
    Q_OBJECT
public:
    explicit CourseSearch(QObject *parent = 0);

    bool home();
    bool login(QString username, QString password);

    QList<QJsonObject> getAllCourse(QString year, QString semester);

    QString getServerNumber();

    QList<QJsonObject> courseFromWeek(QString year, QString semester, QString week, QString time = "*");

signals:
    void print(QString);

public slots:

private:

    QString serverNumber, homeUrl;
    QTextCodec *codec;
    WebAutomaticIndexer *web;
};

#endif // COURSESEARCH_H
