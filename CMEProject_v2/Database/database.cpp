#include "database.h"

#include <QSqlQuery>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>

#include "regularexpression.h"

#define DATABASENAME "CME.db"

Database::Database(QObject *parent)
    : QObject(parent)
{
    initialization();

    /*QJsonObject json;
    json["year"] = "105";
    json["semester"] = "2";
    json["electiveCode"] = "26";
    json["courseName"] = "MATH104";
    json["asd"] = "123";
    qDebug() << addCourseAnalysis(json);*/
    /*QList<QJsonObject> jsonList;
    jsonList << json;
    addCourseCourseSearch(jsonList);*/
    //qDebug() << getCourse(json);
    //qDebug() << getTableColumnName("Course1031");
}

bool Database::addNidAccount(QString nid, QString password)
{
    nid = nid.toUpper();

    QStringList strList;
    strList << "username" << nid;
    strList << "password" << password;
    if(!insertValue("NID", strList)){
        if(db.open()){
            QSqlQuery query(db);
            QString queStr = "SELECT password, id "
                    "FROM NID "
                    "WHERE username = '" + nid + "'";
            query.exec(queStr);
            if (query.next()) {
                QString password_d = query.value(0).toString();
                QString id_d_NID = query.value(1).toString();
                if(password_d != password){
                    //user change password
                    QStringList strList;
                    strList << "NID_id" << id_d_NID;
                    strList << "historicalPassword" << password_d;
                    insertValue("NIDHistoricalData", strList);

                    //update NID table
                    QSqlQuery query(db);
                    QString queStr = "UPDATE NID "
                            "SET password = '" + password + "' "
                            "WHERE username = '" + nid + "'";
                    query.exec(queStr);
                }
            }
        }
    }
    else{
        return true;
    }
    return false;
}

QJsonObject Database::getCourseLearningPortfolio(QJsonObject jsonObject)
{
    QJsonObject retValue;
    QString year = jsonObject.take("year").toString();
    QString semester = jsonObject.take("semester").toString();;

    QString tableName = "CourseLearningPortfolio" + year + semester;

    QSqlQuery query;
    QString queStr = "SELECT * FROM " + tableName + " WHERE";
    for(int i=0;i<jsonObject.size();i++){
        queStr += " " + jsonObject.keys().at(i) + "=\'" + jsonObject[jsonObject.keys().at(i)].toString() + "\'";
        if(i != jsonObject.size() - 1){
            queStr += " AND";
        }
    }
    //qDebug() << queStr;

    if(query.exec(queStr)){
        if(query.next()) {
            QStringList tableTitles = getTableColumnName(tableName);
            //qDebug() << "tableTitles size" << tableTitles.size();
            for(int i=0;i<tableTitles.size();i++){
                //qDebug() << tableTitles.at(i) << query.value(i).toString();
                retValue[static_cast<QString>(tableTitles.at(i))] = query.value(i).toString();
            }
        }
    }

    return retValue;
}

QList<QJsonObject> Database::getCourseCourseSearch(QJsonObject jsonObject)
{
    QList<QJsonObject> retValue;
    QString year = jsonObject.take("year").toString();
    QString semester = jsonObject.take("semester").toString();;

    QString tableName = "CourseCourseSearch" + year + semester;

    QSqlQuery query;
    QString queStr = "SELECT * FROM " + tableName + " WHERE";
    for(int i=0;i<jsonObject.size();i++){
        queStr += " " + jsonObject.keys().at(i) + "='" + jsonObject[jsonObject.keys().at(i)].toString() + '\'';
        if(i != jsonObject.size() - 1){
            queStr += " AND";
        }
    }
    //qDebug() << queStr;

    if(query.exec(queStr)){
        while(query.next()) {
            QStringList tableTitles = getTableColumnName(tableName);
            //qDebug() << "tableTitles size" << tableTitles.size();
            QJsonObject json;
            for(int i=0;i<tableTitles.size();i++){
                //qDebug() << tableTitles.at(i) << query.value(i).toString();
                json[static_cast<QString>(tableTitles.at(i))] = query.value(i).toString();
            }
            retValue << json;
        }
    }

    return retValue;
}

QJsonObject Database::getCourseAnalysis(QJsonObject jsonObject)
{
    QJsonObject retValue;
    QString year = jsonObject.take("year").toString();
    QString semester = jsonObject.take("semester").toString();;

    QString tableName = "CourseAnalysis" + year + semester;

    QSqlQuery query;
    QString queStr = "SELECT * FROM " + tableName + " WHERE";
    for(int i=0;i<jsonObject.size();i++){
        queStr += " " + jsonObject.keys().at(i) + "=\'" + jsonObject[jsonObject.keys().at(i)].toString() + "\'";
        if(i != jsonObject.size() - 1){
            queStr += " AND";
        }
    }
    //qDebug() << queStr;

    if(query.exec(queStr)){
        if(query.next()) {
            QStringList tableTitles = getTableColumnName(tableName);
            //qDebug() << "tableTitles size" << tableTitles.size();
            for(int i=0;i<tableTitles.size();i++){
                //qDebug() << tableTitles.at(i) << query.value(i).toString();
                retValue[static_cast<QString>(tableTitles.at(i))] = query.value(i).toString();
            }
        }
    }

    return retValue;
}

bool Database::addCourseAnalysis(QJsonObject jsonObject)
{
    QString year = jsonObject.take("year").toString();
    QString semester = jsonObject.take("semester").toString();;

    QString tableName = "CourseAnalysis" + year + semester;
    QStringList defaultColumn;
    defaultColumn << "electiveCode" << "INTEGER PRIMARY KEY";
    defaultColumn << "courseName" << "TEXT";
    defaultColumn << "teacher" << "TEXT";
    defaultColumn << "friendliness" << "TEXT";

    if(!year.isEmpty() && !semester.isEmpty()){
        //check table exist
        if(!isTableExist(tableName)){
            createTable(tableName, defaultColumn);
        }

        //check column exist
        QStringList columns = getTableColumnName(tableName);
        for(int i=0;i<jsonObject.size();i++){
            if(!columns.contains(jsonObject.keys().at(i))){
                addColumn(tableName, jsonObject.keys().at(i));
            }
        }

        //check value exist


        //insert value
        return insertValue(tableName, jsonObject);
    }

    return false;
}

bool Database::addCourseLearningPortfolio(QJsonObject jsonObject)
{
    //qDebug() << "add course" << jsonObject;
    QString year = jsonObject.take("year").toString();
    QString semester = jsonObject.take("semester").toString();;

    QString tableName = "CourseLearningPortfolio" + year + semester;
    QStringList defaultColumn;
    defaultColumn << "courseCode" << "TEXT UNIQUE";
    //defaultColumn << "electiveCode" << "TEXT UNIQUE";
    defaultColumn << "courseName" << "TEXT";

    if(!year.isEmpty() && !semester.isEmpty()){
        //check table exist
        if(!isTableExist(tableName)){
            createTable(tableName, defaultColumn);
        }

        //check column exist
        QStringList columns = getTableColumnName(tableName);
        for(int i=0;i<jsonObject.size();i++){
            if(!columns.contains(jsonObject.keys().at(i))){
                addColumn(tableName, jsonObject.keys().at(i));
            }
        }

        //check value exist


        //insert value
        return insertValue(tableName, jsonObject);
    }

    return false;
}

bool Database::addCourseCourseSearch(QJsonObject jsonObject)
{
    QString year = jsonObject.take("year").toString();
    QString semester = jsonObject.take("semester").toString();;

    QString tableName = "CourseCourseSearch" + year + semester;
    QStringList defaultColumn;
    defaultColumn << "electiveCode" << "INTEGER PRIMARY KEY";
    defaultColumn << "courseCode" << "TEXT UNIQUE";
    defaultColumn << "courseName" << "TEXT";

    if(!year.isEmpty() && !semester.isEmpty()){
        //check table exist
        if(!isTableExist(tableName)){
            createTable(tableName, defaultColumn);
        }

        //check column exist
        QStringList columns = getTableColumnName(tableName);
        for(int i=0;i<jsonObject.size();i++){
            if(!columns.contains(jsonObject.keys().at(i))){
                addColumn(tableName, jsonObject.keys().at(i));
            }
        }

        //check value exist


        //insert value
        return insertValue(tableName, jsonObject);
    }

    return false;
}

bool Database::addCourseGrade(QJsonObject jsonObject)
{
    QString year = jsonObject.take("year").toString();
    QString semester = jsonObject.take("semester").toString();;

    QString tableName = "CourseGrade" + year + semester;
    QStringList defaultColumn;
    defaultColumn << "courseCode" << "TEXT";
    defaultColumn << "studentID" << "TEXT";

    if(!year.isEmpty() && !semester.isEmpty()){
        //check table exist
        if(!isTableExist(tableName)){
            createTable(tableName, defaultColumn);
        }

        //check column exist
        QStringList columns = getTableColumnName(tableName);
        for(int i=0;i<jsonObject.size();i++){
            if(!columns.contains(jsonObject.keys().at(i))){
                addColumn(tableName, jsonObject.keys().at(i));
            }
        }

        //insert value
        return insertValue(tableName, jsonObject);
    }

    return false;
}

bool Database::insertValue(QString tableName, QJsonObject jsonObject)
{
    if(jsonObject.size() == 0)
        return false;

    if(db.open()){
        QSqlQuery query(db);
        QString queStr = "INSERT INTO " + tableName + " (";
        for(int i=0;i<jsonObject.size();i++){
            queStr+=jsonObject.keys().at(i) + ",";
        }
        queStr.remove(queStr.size()-1, 1);
        queStr += ") VALUES (";
        for(int i=0;i<jsonObject.size();i++){
            queStr+="?,";
        }
        queStr.remove(queStr.size()-1, 1);
        queStr += ")";
        //qDebug() << queStr;
        query.prepare(queStr);

        for(int i=0;i<jsonObject.size();i++){
            query.addBindValue(static_cast<QString>(jsonObject[jsonObject.keys().at(i)].toString()));
        }

        //qDebug() << queStr;
        return query.exec();
    }

    return false;
}

bool Database::insertValue(QString tableName, QStringList strList)
{
    if(strList.size()<2)
        return false;

    if(db.open()){
        QSqlQuery query(db);
        QString queStr = "INSERT INTO " + tableName + " (";
        for(int i=1;i<strList.size();i+=2){
            queStr+=strList.at(i-1) + ",";
        }
        queStr.remove(queStr.size()-1, 1);
        queStr += ") VALUES (";
        for(int i=1;i<strList.size();i+=2){
            queStr+="?,";
        }
        queStr.remove(queStr.size()-1, 1);
        queStr += ")";
        //qDebug() << queStr;
        query.prepare(queStr);

        for(int i=1;i<strList.size();i+=2){
            query.addBindValue(static_cast<QString>(strList.at(i)));
        }

        return query.exec();
    }

    return false;
}

bool Database::isTableExist(QString tableName)
{
    if(db.open()){
        QSqlQuery query(db);
        QString queStr = "SELECT name FROM sqlite_master WHERE TYPE='table' AND name=\'" + tableName + "\'";
        if(query.exec(queStr)){
            if(query.next()){
                QString ret = query.value(0).toString();
                return !ret.isNull();
            }
        }
    }

    return false;
}

bool Database::createTable(QString tableName, QStringList strList)
{
    if(db.open()){
        QSqlQuery query(db);
        QString queStr = "CREATE TABLE " + tableName + " (";
        for(int i=1;i<strList.size();i+=2){
            queStr += "\'" + strList.at(i-1) + "\' " + strList.at(i) + ",";
        }
        if(strList.size()>=2)
            queStr.remove(queStr.size()-1, 1);
        queStr += ")";
        if(query.exec(queStr)){
            return true;
        }
    }

    return false;
}

bool Database::addColumn(QString tableName, QString columnName)
{
    if(db.open()){
        QSqlQuery query(db);
        QString queStr = "ALTER TABLE " + tableName + " ADD " + columnName + " TEXT";
        if(query.exec(queStr)){
            return true;
        }
    }

    return false;
}

QStringList Database::getTableColumnName(QString tableName)
{
    QStringList retValue;
    if(db.open()){
        QSqlQuery query(db);
        QString queStr = "SELECT sql FROM sqlite_master WHERE type=\'table\' AND name = \'" + tableName + "\'";
        if(query.exec(queStr)){
            if(query.next()){
                QString ret = query.value(0).toString();
                //qDebug() << ret;
                ret.remove(0, 15 + tableName.size());
                ret.push_front(",");

                /*{
                    QString pattern = ",\'(.+?)\' ";
                    QList<QStringList> strLL = RegularExpression::match(pattern, ret);
                    foreach (QStringList strList, strLL) {
                        if(!strList.isEmpty())
                            retValue << strList.first();
                    }
                }*/
                {
                    QString pattern = ",(.+?) ";
                    QList<QStringList> strLL = RegularExpression::match(pattern, ret);
                    foreach (QStringList strList, strLL) {
                        if(!strList.isEmpty()){
                            QString str = strList.first();
                            //check like "'electiveCode'"
                            if(str.size() >= 2){
                                if(str.at(0) == "'"){
                                    str.remove(0,1);
                                    str.remove(str.size()-1,1);
                                }
                            }
                            //check like " score_60"
                            if(!str.isEmpty()){
                                if(str.at(0) == " "){
                                    str.remove(0,1);
                                }
                            }
                            retValue << str;
                        }
                    }
                }
                //qDebug() << ret;
            }
        }
    }
    else{
        qDebug() << "db open error";
    }

    return retValue;
}

void Database::initialization()
{
    static bool isFirstOpen = true;
    if(isFirstOpen){
        isFirstOpen = false;

        QFileInfo databasFile(DATABASENAME);
        if(!databasFile.exists()){
            //no find database
            //PRINT("database not exist")
            qDebug() << "can't find database";
        }

        if(!QSqlDatabase::database().isValid()){
            //set database information
            QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
            database.setDatabaseName(DATABASENAME);
            //PRINT(QString("setDatabaseName ") + DATABASENAME)
        }

        db = QSqlDatabase::database();
        if(!isTableExist("NID")){
            QStringList strList;
            strList << "id" << "INTEGER PRIMARY KEY AUTOINCREMENT";
            strList << "username" << "TEXT UNIQUE";
            strList << "password" << "TEXT";
            if(!createTable("NID", strList)){
                qDebug() << "can't create table: NID";
            }
        }
        if(!isTableExist("NIDHistoricalData")){
            QStringList strList;
            strList << "id" << "INTEGER PRIMARY KEY AUTOINCREMENT";
            strList << "NID_id" << "REFERENCES NID (id)";
            strList << "historicalPassword" << "TEXT";
            if(!createTable("NIDHistoricalData", strList)){
                qDebug() << "can't create table: NIDHistoricalData";
            }
        }
        /*if(!isTableExist("CourseSearchCourse")){
            QStringList strList;
            strList << "id" << "INTEGER PRIMARY KEY AUTOINCREMENT";
            strList << "electiveCode" << "INTRGER UNIQUE";
            strList << "courseName" << "TEXT";
            strList << "credit" << "TEXT";
            strList << "isRequired" << "TEXT";
            strList << "hasMidtermExam" << "TEXT";
            strList << "hasFinalExam" << "TEXT";
            strList << "teacher" << "TEXT";
            strList << "numberOfPeople" << "TEXT";
            strList << "totalOfPeople" << "TEXT";
            strList << "timeClass" << "TEXT";
            strList << "courseClass" << "TEXT";
            strList << "courseCode" << "TEXT";
            if(!createTable("CourseSearchCourse", strList)){
                qDebug() << "can't create table: CourseSearchCourse";
            }
        }
        if(!isTableExist("CourseLearningProtfolie")){
            QStringList strList;
            strList << "id" << "INTEGER PRIMARY KEY AUTOINCREMENT";
            strList << "CourseSearchCourse_courseCode" << "TEXT UNIQUE";
            strList << "year" << "TEXT";
            strList << "semester" << "TEXT";
            strList << "numberOfPeople" << "TEXT";
            strList << "average" << "TEXT";
            for(int i=1;i<=20;i++)
                strList << QString::asprintf("numberAt%d", i*5) << "TEXT";
            if(!createTable("CourseLearningProtfolie", strList)){
                qDebug() << "can't create table: CourseLearningProtfolie";
            }
        }*/
    }
    else
        db = QSqlDatabase::database();
}
