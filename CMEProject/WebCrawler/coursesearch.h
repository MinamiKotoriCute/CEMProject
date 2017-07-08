#ifndef COURSESEARCH_H
#define COURSESEARCH_H

#include <QObject>

class WebAutomaticIndexer;

class CourseSearch : public QObject
{
    Q_OBJECT
public:
    explicit CourseSearch(QObject *parent = 0);
    ~CourseSearch();

    QString home();
    QString login(QString username, QString password);
    QString courseFromName(QString year, QString semester, QString courseName);
    QString courseFromNameTeacher(QString year, QString semester, QString courseName, QString teacher);
    QString courseFromWeek(QString year, QString semester, QString week);
    QString courseFromElectiveCode(QString year, QString semester, QString electiveCode);

    QString electiveCoursesList(QString year, QString semester);
    //QList<Course> coursesList(QString year, QString semester, QString item, QString text);
    QString coursesList(QString year, QString semester, QString item, QString text);

    QList<QStringList> electiveCoursesAnalysis();

    void addAll(QString);

signals:
    void print(QString);

public slots:

private:
    QString serverNumber, homeUrl;
    QTextCodec *codec;
    WebAutomaticIndexer *web;
};

#endif // COURSESEARCH_H
