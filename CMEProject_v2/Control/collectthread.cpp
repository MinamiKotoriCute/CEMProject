#include "collectthread.h"

#include <QJsonObject>
#include <QDebug>

#include "WebCrawler/coursesearch.h"
#include "WebCrawler/learningprotfolio.h"
#include "Database/database.h"

#define NOW_YEAR 105
#define NOW_SEMESTER 1
#define SEARCH_NUMBER 15

CollectThread::CollectThread(QString username, QString password, int status, QObject *parent) :
    QThread(parent),
    username(username),
    password(password),
    status(status)
{

}

void CollectThread::run()
{
    Database db;

    switch (status) {
    case 0:{
        LearningProtfolio *learningProtfolio = new LearningProtfolio;
        learningProtfolio->home();
        if(learningProtfolio->login(username, password)){
            QList<QJsonObject> jsonList = learningProtfolio->test();
            int i = 0;
            foreach (QJsonObject jsonObject, jsonList) {
                if(i%2 == 0){
                    db.addCourseLearningPortfolio(jsonObject);
                }
                else{
                    db.addCourseGrade(jsonObject);
                }
                i++;
            }
        }
        break;
    }
    case 1:{
        CourseSearch courseSearch;
        courseSearch.home();

        int year = NOW_YEAR;
        int semester = NOW_SEMESTER;
        for(int i=0;i<SEARCH_NUMBER;i++){
            //a week have 7 day
            for(int j=1;j<=7;j++){
                emit print(QString::asprintf("%d-%d week:%d", year, semester, j));
                qDebug() << QString::asprintf("%d-%d week:%d", year, semester, j);
                QList<QJsonObject> courseList = courseSearch.courseFromWeek(QString::number(year), QString::number(semester), QString::number(j));
                foreach (QJsonObject course, courseList) {
                    db.addCourseCourseSearch(course);
                }
            }

            semester--;
            if(semester == 0){
                semester = 2;
                year--;
            }
        }
        break;
    }
    case 2:{
        int year_n = NOW_YEAR;
        int semester_n = NOW_SEMESTER;
        semester_n++;
        if(semester_n == 3){
            semester_n = 1;
            year_n++;
        }

        CourseSearch courseSearch;
        courseSearch.home();

        for(int j=1;j<=7;j++){
            emit print(QString::asprintf("%d-%d week:%d", year_n, semester_n, j));
            qDebug() << QString::asprintf("%d-%d week:%d", year_n, semester_n, j);
            QList<QJsonObject> courseList = courseSearch.courseFromWeek(QString::number(year_n), QString::number(semester_n), QString::number(j));
            foreach (QJsonObject course, courseList) {
                double totalPeople = 0;
                double lastPeople = 0;
                int year = NOW_YEAR;
                int semester = NOW_SEMESTER;
                int courseCount = 0;

                for(int i=0;i<SEARCH_NUMBER;i++){
                    QJsonObject course_now;
                    course_now["year"] = QString::number(year);
                    course_now["semester"] = QString::number(semester);
                    course_now["teacher"] = course["teacher"].toString();
                    course_now["courseName"] = course["courseName"].toString();
                    QList<QJsonObject> courseOldList = db.getCourseCourseSearch(course_now);
                    courseCount += courseOldList.size();
                    foreach (QJsonObject course_old, courseOldList) {
                        totalPeople+=course_old["defaultNumberOfPeople"].toString().toDouble();
                        lastPeople+=course_old["lastNumberOfPeople"].toString().toDouble();
                    }

                    semester--;
                    if(semester == 0){
                        semester = 2;
                        year--;
                    }
                }

                QJsonObject course_now;
                course_now["year"] = QString::number(year_n);
                course_now["semester"] = QString::number(semester_n);
                course_now["teacher"] = course["teacher"].toString();
                course_now["courseName"] = course["courseName"].toString();
                course_now["electiveCode"] = course["electiveCode"].toString();
                course_now["lastNumberOfPeople"] = QString::number(lastPeople);
                course_now["defaultNumberOfPeople"] = QString::number(totalPeople);
                course_now["numberOfCourse"] = QString::number(courseCount);
                if(totalPeople != 0){
                    course_now["friendliness"] = QString::number(lastPeople/totalPeople);
                }
                db.addCourseAnalysis(course_now);
                //qDebug() << courseCount << totalPeople << lastPeople << lastPeople/totalPeople << course["courseName"].toString() << course["teacher"].toString();
            }
        }

        break;
    }
    default:
        break;
    }

    qDebug() << "finish";
}
