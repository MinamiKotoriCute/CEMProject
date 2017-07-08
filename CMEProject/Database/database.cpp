#include "database.h"

#include <QSqlQuery>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>

#define DATABASENAME "CME"

#define PRINT(text)\
    emit print(text);

/*void _select(){
    QSqlQuery query;
    qDebug() << query.exec("SELECT First_Name FROM Customer");
    while (query.next()) {
        QString First_Name = query.value(0).toString();
        qDebug() << First_Name;
    }
}/*

/*void _insert(){
    QStringList strList;
    strList << "First_Name" << "ASD";
    insertValue("Customer", strList);
}*/

Database::Database(QObject *parent) : QObject(parent)
{
    initialization();
    /*addNidAccount("apply", "a1");
    addNidAccount("banana", "b1");
    addNidAccount("apply", "a2");*/
}

bool Database::addNidAccount(QString nid, QString password)
{
    QStringList strList;
    strList << "username" << nid;
    strList << "password" << password;
    qDebug() << 1;
    if(!insertValue("NID", strList)){
        qDebug() << 2;
        if(db.open()){
            qDebug() << 3;
            QSqlQuery query(db);
            QString queStr = "SELECT password, id "
                    "FROM NID "
                    "WHERE username = '" + nid + "'";
            query.exec(queStr);
            if (query.next()) {

                qDebug() << 4;
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
        qDebug() << 6;
        return true;
    }
    qDebug() << 5;
    return false;
}

void Database::addCourse(QStringList information)
{
    if(information.size() != 8){
        qDebug() << "error" << "void Database::addCourse";
        return;
    }

    QStringList strList;
    strList << "electiveCode" << information.at(0);
    strList << "courseName" << information.at(1);
    strList << "credit" << information.at(2);
    strList << "isRequired" << information.at(3);
    strList << "hasMidtermExam" << information.at(4);
    strList << "hasFinalExam" << information.at(5);
    strList << "detail" << information.at(6);
    strList << "numberOfPeople" << information.at(7);

    insertValue("CourseSearchCourse", strList);
    //qDebug() << "inser course" << insertValue("CourseSearchCourse", strList);
}

void Database::addCourse(Course course)
{
    QStringList strList;
    strList << "electiveCode" << QString::number(course.electiveCode());
    strList << "courseName" << course.courseName();
    strList << "credit" << course.credit();
    strList << "isRequired" << course.isRequired();
    strList << "hasMidtermExam" << course.hasMidtermExam();
    strList << "hasFinalExam" << course.hasFinalExam();
    strList << "teacher" << course.teacher();
    strList << "numberOfPeople" << course.numberOfPeople();
    strList << "totalOfPeople" << course.totalOfPeople();
    strList << "timeClass" << course.timeClass();
    strList << "courseClass" << course.courseClass();
    strList << "courseCode" << course.courseCode();

    insertValue("CourseSearchCourse", strList);
}

Course Database::courseInformation(QString electiveCode, QString year, QString semeter)
{
    Course course;
    QSqlQuery query;
    query.exec("SELECT * FROM CourseSearchCourse" + year + semeter + " WHERE electiveCode=" + electiveCode);
    int i = 0;
    if (query.next()) {
        switch (i) {
        case 1: course.set_electiveCode(query.value(i).toInt()); break;
        case 2: course.set_courseName(query.value(i).toString()); break;
        case 3: course.set_credit(query.value(i).toString()); break;
        case 4: course.set_isRequired(query.value(i).toString()); break;
        case 5: course.set_hasMidtermExam(query.value(i).toString()); break;
        case 6: course.set_hasFinalExam(query.value(i).toString()); break;
        case 7: course.set_teacher(query.value(i).toString()); break;
        case 8: course.set_numberOfPeople(query.value(i).toString()); break;
        case 9: course.set_totalOfPeople(query.value(i).toString()); break;
        case 10: course.set_timeClass(query.value(i).toString()); break;
        case 11: course.set_courseClass(query.value(i).toString()); break;
        case 12: course.set_courseCode(query.value(i).toString()); break;
        default:
            break;
        }
        i++;
    }
    return course;
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

void Database::checkTable(QString tableName, QStringList strList)
{
    /*QSqlQuery query(db);
    QString queStr = "select sql from sqlite_master where name = '" + tableName + "'";
    qDebug() << query.exec(queStr);
    if (query.next()) {
        QString createInformation = query.value(0).toString();
    }*/
}

void Database::initialization()
{
    static bool isFirstOpen = true;
    if(isFirstOpen){
        isFirstOpen = false;

        QFileInfo databasFile(DATABASENAME);
        if(!databasFile.exists()){
            //no find database
            PRINT("database not exist")
        }

        if(!QSqlDatabase::database().isValid()){
            //set database information
            QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
            database.setDatabaseName(DATABASENAME);
            PRINT(QString("setDatabaseName ") + DATABASENAME)
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
        if(!isTableExist("CourseSearchCourse")){
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
        }
    }
    else
        db = QSqlDatabase::database();
}
