#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

#include <QSqlDatabase>
#include <QJsonObject>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = 0);

    QJsonObject getCourseLearningPortfolio(QJsonObject jsonObject);
    QList<QJsonObject> getCourseCourseSearch(QJsonObject jsonObject);
    QJsonObject getCourseAnalysis(QJsonObject jsonObject);

    bool addCourseAnalysis(QJsonObject jsonObject);
    bool addNidAccount(QString nid, QString password);
    bool addCourseLearningPortfolio(QJsonObject jsonObject);
    bool addCourseCourseSearch(QJsonObject jsonObject);
    bool addCourseGrade(QJsonObject jsonObject);

    bool insertValue(QString tableName, QJsonObject jsonObject);

signals:

public slots:

private:
    void initialization();

    bool insertValue(QString tableName, QStringList strList);
    bool isTableExist(QString tableName);
    bool createTable(QString tableName, QStringList strList);
    bool addColumn(QString tableName, QString columnName);
    QStringList getTableColumnName(QString tableName);

    QSqlDatabase db;
};

#endif // DATABASE_H
