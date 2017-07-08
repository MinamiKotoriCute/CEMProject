#include "collectthread.h"

#include <QDebug>

#include "learningprotfolio.h"
#include "coursesearchanalyzer.h"
#include "database.h"

#define NOWYEAR 105
#define NOWSEMESTER 1

CollectThread::CollectThread(QString username, QString password) :
    username(username),
    password(password),
    status(0)
{

}

void CollectThread::run()
{
    Database db;

    /*CourseSearchAnalyzer courseSearchAnalyzer;
    courseSearchAnalyzer.home();
    if(courseSearchAnalyzer.login(username, password)){
        int year = NOWYEAR;
        int semester = NOWSEMESTER;
        QList<Course> electiveCoursesLists = courseSearchAnalyzer.electiveCoursesList(QString::number(year), QString::number(semester));
        while(!electiveCoursesLists.isEmpty()){
            //everyone class of now
            foreach(Course course, electiveCoursesLists){

                Database db;
                db.addCourse(course);
            }
            --semester;
            if(semester == 0){
                semester = 2;
                --year;
            }
            electiveCoursesLists = courseSearchAnalyzer.electiveCoursesList(QString::number(year), QString::number(semester));
        }
    }*/

    LearningProtfolio learningPortfolio;
    learningPortfolio.home();
    if(learningPortfolio.login(username, password)){
        learningPortfolio.test();
    }
}
