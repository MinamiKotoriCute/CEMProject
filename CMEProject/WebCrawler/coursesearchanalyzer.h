#ifndef COURSESEARCHANALYZER_H
#define COURSESEARCHANALYZER_H

#include <QObject>

#include "webcrawler_global.h"

//#include "course.h"
#include "../course/course.h"

class CourseSearch;

class WEBCRAWLERSHARED_EXPORT CourseSearchAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit CourseSearchAnalyzer(QObject *parent = 0);

    bool home();
    bool login(QString username, QString password);
    QList<Course> electiveCoursesList(QString year, QString semester);
    QList<Course> coursesList(QString year, QString semester, QString text, QString electiveCode = QString(), QString teacher = QString());

    Course courseNameFromElectiveCode(QString year, QString semester, QString electiveCode);
    QList<Course> courseListFromCourse(Course course);

    double coursePopularity(QString electiveCode);

signals:
    void print(QString);

public slots:

private:
    CourseSearch *courseSearch;
};

#endif // COURSESEARCHANALYZER_H
