#ifndef DATABASE_H
#define DATABASE_H

#include "database_global.h"

#include <QSqlDatabase>
#include <QObject>

//#include "course.h"
#include "../course/course.h"

class DATABASESHARED_EXPORT Database : public QObject
{
    Q_OBJECT
public:
    Database(QObject *parent = 0);
    bool addNidAccount(QString nid, QString password);

    void addCourse(QStringList information);
    void addCourse(Course course);
    Course courseInformation(QString electiveCode, QString year, QString semeter);

    bool insertValue(QString tableName, QStringList strList);
    //bool isCourseExistLearningProtfolie(QString year, QString semeter, QString courseCode);

signals:
    void print(QString);

private:
    void initialization();

    bool isTableExist(QString tableName);
    bool createTable(QString tableName, QStringList strList);

    void checkTable(QString tableName, QStringList strList);

    QSqlDatabase db;
};

#endif // DATABASE_H
