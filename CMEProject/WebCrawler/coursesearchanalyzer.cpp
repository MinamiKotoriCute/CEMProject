#include "coursesearchanalyzer.h"

#include <QDebug>
#include <QJsonDocument>

#include "coursesearch.h"
#include "regularexpression.h"
#include "expansion.h"
#include "database.h"

CourseSearchAnalyzer::CourseSearchAnalyzer(QObject *parent) :
    QObject(parent),
    courseSearch(new CourseSearch(this))
{
    connect(courseSearch, &CourseSearch::print, this, &CourseSearchAnalyzer::print);
}

bool CourseSearchAnalyzer::home()
{
    courseSearch->home();
    return 0;
}

bool CourseSearchAnalyzer::login(QString username, QString password)
{
    QString html = courseSearch->login(username, password);
    if(html.contains("status\\\":0")){
        qDebug() << "login seccess";
        return true;
    }
    else{
        qDebug() << "login fail";
    }

    return false;
}

QList<Course> CourseSearchAnalyzer::electiveCoursesList(QString year, QString semester)
{
    QString html = courseSearch->electiveCoursesList(year, semester);

    QString pattern = "<TR class=.*?align=center>(?:.+?)</TD>.+?href=\"(.+?)\">(.+?)</a>.+?align=center>(.+?)</TD>.+?align=center>(.+?)</TD>.+?align=center>(.+?)</TD>.+?align=center>(.+?)</TD>.+?align=center>(.+?)</TD>.+?</TR>";
    QList<QStringList> strLL = RegularExpression::match(pattern, html);
    strLL = RegularExpression::simplified(strLL);

    QList<Course> courses;

    foreach(QStringList strList, strLL){
        if(strList.size() < 7)
            continue;
        QString courseDetailUrl = strList.at(0);
        QString courseName = _split(strList.at(1), " ", 1);
        QString courseCode = _split(strList.at(1), " ", 0);
        QString courseClassName = strList.at(2);
        QString courseDetail = strList.at(3);
        QString electiveCode = strList.at(4);
        QString courseCredit = strList.at(5);
        QString isRequired = strList.at(6);
        QStringList courseInformationList = courseName.split(" ");
        QString className;
        if(courseInformationList.size() >= 2)
            className = courseInformationList.at(1);
        QString teacherName = _split(courseDetail, " ", -2, 3);
        QString timeclass;
        QStringList courseDetailList = courseDetail.split(" ");
        for(int i=0;i<courseDetailList.size()-1;i++)
            timeclass+=courseDetailList.at(i) + " ";
        if(courseDetailList.size()>1)
            timeclass.remove(timeclass.size()-1, 1);

        Course course;
        course.set_courseCode(courseCode);
        course.set_courseName(courseName);
        course.set_credit(courseCredit);
        course.set_courseClass(courseClassName);
        course.set_teacher(teacherName);
        course.set_electiveCode(electiveCode.toInt());
        course.set_isRequired(isRequired);
        course.set_timeClass(timeclass);
        courses.push_back(course);

        //calculate 8 semester
        /*int totalPeople = 0;
        int remainPeople = 0;
        for(int i=0;i<7;i++){
            QString yearStr = QString::number(year-(i+1)/2);
            QString semesterStr = QString::number(semester+(i%2));
            QList<QStringList> targetCourse = courseSearch.coursesList(yearStr, semesterStr, "2", className, teacherName);
            foreach (QStringList strList, targetCourse) {
                if(strList.size() == 10){
                    QString numberOfPeole = strList.at(9);
                    QStringList numberOfPeole_tok = numberOfPeole.split("/");
                    if(numberOfPeole_tok.size()==2){
                        QString numberOfPeole_total = numberOfPeole_tok.at(0);
                        QString numberOfPeole_now = numberOfPeole_tok.at(1);
                        totalPeople += numberOfPeole_total.toInt();
                        remainPeople += numberOfPeole_now.toInt();
                    }
                    //qDebug() << numberOfPeole << QString::asprintf("%d-%d", year-(i+1)/2, semester+(i%2));
                }
            }
        }
        QStringList courseTotalInformation;
        courseTotalInformation << electiveCode << className << teacherName << QString::number((double)remainPeople/totalPeople);
        retValue << courseTotalInformation;*/

        //qDebug() << className << teacherName << totalPeople << remainPeople;
    }

    return courses;
}

QList<Course> CourseSearchAnalyzer::coursesList(QString year, QString semester, QString text, QString electiveCode, QString teacher)
{
    QString html = courseSearch->coursesList(year, semester, "2", text);

    QString pattern = "<TD width=50 >(.+?)</TD>.+?href=\"(.+?)\">(.+?)</A>.+?<TD >(\\d+?)</TD>.+?<TD >(.+?)</TD>.+?<TD >(.+?)</TD>.+?<TD >(.+?)</TD>.+?<TD >(.+?)</TD>.+?\">(.+?)</A>.+?<TD >(.+?)</TD>.+?<TD >(.+?)</TD>.+?<TD >";
    //("0923", "coursesummary.jsp?strCourseId=1051CB2613199212001MKT344aCB26139", "MKT344 日本商情與策略", "3", "選修", "否", "否", "否", "行銷三甲", "(二)02-04 人607 黃煇慶", "60/31")
    QList<QStringList> strLL = RegularExpression::match(pattern, html);
    strLL = RegularExpression::simplified(strLL);

    QList<Course> courses;

    foreach(QStringList strList, strLL){
        if(strList.size() < 11)
            continue;

        QString timeclass;
        QString teacher;
        QStringList courseDetailList = strList.at(9).split(" ");
        if(courseDetailList.size()>=1){
            teacher = courseDetailList.takeLast();
        }
        for(int i=0;i<courseDetailList.size();i++)
            timeclass+=courseDetailList.at(i) + " ";
        if(!courseDetailList.isEmpty())
            timeclass.remove(timeclass.size(), 1);
        QString totalOfPeople;
        QString numberOfPeople;
        QStringList numberOfPeopleList = strList.at(10).split("/");
        if(numberOfPeopleList.size()==2){
            totalOfPeople = numberOfPeopleList.at(0);
            numberOfPeople = numberOfPeopleList.at(1);
        }

        Course course;
        course.set_electiveCode(QString(strList.at(0)).toInt());
        course.set_courseName(strList.at(2));
        course.set_credit(strList.at(3));
        course.set_isRequired(strList.at(4));
        course.set_hasMidtermExam(strList.at(5));
        course.set_hasFinalExam(strList.at(6));
        course.set_courseClass(strList.at(8));
        course.set_teacher(teacher);
        course.set_timeClass(timeclass);
        course.set_totalOfPeople(totalOfPeople);
        course.set_numberOfPeople(numberOfPeople);
        course.set_teacher(teacher);
        course.set_electiveCode(electiveCode.toInt());

        Database db;
        db.addCourse(course);
        if(!electiveCode.isEmpty())
            if(electiveCode != strList.at(0))
                continue;

        if(!teacher.isEmpty())
            if(teacher != course.teacher())
                continue;
        courses.push_back(course);

        //calculate 8 semester
        /*int totalPeople = 0;
        int remainPeople = 0;
        for(int i=0;i<7;i++){
            QString yearStr = QString::number(year-(i+1)/2);
            QString semesterStr = QString::number(semester+(i%2));
            QList<QStringList> targetCourse = courseSearch.coursesList(yearStr, semesterStr, "2", className, teacherName);
            foreach (QStringList strList, targetCourse) {
                if(strList.size() == 10){
                    QString numberOfPeole = strList.at(9);
                    QStringList numberOfPeole_tok = numberOfPeole.split("/");
                    if(numberOfPeole_tok.size()==2){
                        QString numberOfPeole_total = numberOfPeole_tok.at(0);
                        QString numberOfPeole_now = numberOfPeole_tok.at(1);
                        totalPeople += numberOfPeole_total.toInt();
                        remainPeople += numberOfPeole_now.toInt();
                    }
                    //qDebug() << numberOfPeole << QString::asprintf("%d-%d", year-(i+1)/2, semester+(i%2));
                }
            }
        }
        QStringList courseTotalInformation;
        courseTotalInformation << electiveCode << className << teacherName << QString::number((double)remainPeople/totalPeople);
        retValue << courseTotalInformation;*/

        //qDebug() << className << teacherName << totalPeople << remainPeople;
    }

    return courses;
}

Course CourseSearchAnalyzer::courseNameFromElectiveCode(QString year, QString semester, QString electiveCode)
{
    QString html = courseSearch->courseFromElectiveCode(year, semester, electiveCode);
    _save(html.toLatin1());

    //(("1300", "FCME247 纖維材料學", "2.0", "必修", "否", "否", "否", "纖複二甲", "(一)03-04 忠210 林佳弘", "65.0", "0.0"))
    QString pattern = "\\\\\\\"scr_selcode\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"sub_name\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"scr_credit\\\\\\\":(.+?),.*?"
                      "\\\\\\\"scj_scr_mso\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"scr_examid\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"scr_examfn\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"scr_exambf\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"cls_name\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"scr_period\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"scr_precnt\\\\\\\":(.+?),.*?"
                      "\\\\\\\"scr_acptcnt\\\\\\\":(.+?),";
    QList<QStringList> strLL = RegularExpression::match(pattern, html);
    strLL = RegularExpression::simplified(strLL);
    qDebug() << strLL;

    foreach (QStringList strList, strLL) {
        if(strList.size() == 11){
            QString electiveCode = strList.at(0);
            QString courseCode = _split(strList.at(1), " ", 0);
            QString courseName = _split(strList.at(1), " ", 1);
            for(int i=0;i<courseName.size();i++){
                if(courseName.at(i) == '\\'){
                    courseName.remove(i, courseName.size()-i);
                }
            }
            QString credit = strList.at(2);
            QString isRequired = strList.at(3);
            QString hasMidtermExam = strList.at(4);
            QString hasFinalExam = strList.at(5);
            QString hasBefore = strList.at(6);
            QString courseClass = strList.at(7);
            QString timeClass = _split(strList.at(8), " ", -12, 1);
            QString teacher = _split(strList.at(8), " ", -2);
            QString totalOfPeople = strList.at(9);
            QString numberOfPeople = strList.at(10);

            Course course;
            course.set_electiveCode(electiveCode.toInt());
            course.set_courseName(courseName);
            course.set_credit(credit);
            course.set_isRequired(isRequired);
            course.set_hasMidtermExam(hasMidtermExam);
            course.set_hasFinalExam(hasFinalExam);
            course.set_courseClass(courseClass);
            course.set_teacher(teacher);
            course.set_timeClass(timeClass);
            course.set_totalOfPeople(totalOfPeople);
            course.set_numberOfPeople(numberOfPeople);
            course.set_teacher(teacher);
            course.set_courseCode(courseCode);
            course.set_year(year);
            course.set_semester(semester);
            return course;
        }
    }
    /*if(!strLL.isEmpty())
        if(!strLL.first().isEmpty()){
            QStringList strList = static_cast<QString>(strLL.first().first()).split(" ");
            if(strList.size() == 2){
                QString courseCode = strList.at(0);
                QString courseName = strList.at(1);
                return courseName;
            }
        }*/

    return Course();
}

QList<Course> CourseSearchAnalyzer::courseListFromCourse(Course course)
{
    //qDebug() << course.year() << course.semester() << course.courseName() << course.teacher();
    QString html = courseSearch->courseFromNameTeacher(course.year(), course.semester(), course.courseName(), course.teacher());
    _save(html.toLatin1());

    QList<Course> courseList;

    QString pattern = "\\\\\\\"scr_selcode\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"sub_name\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"scr_credit\\\\\\\":(.+?),.*?"
                      "\\\\\\\"scj_scr_mso\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"scr_examid\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"scr_examfn\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"scr_exambf\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"cls_name\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"scr_period\\\\\\\":\\\\\\\"(.+?)\\\\\\\".*?"
                      "\\\\\\\"scr_precnt\\\\\\\":(.+?),.*?"
                      "\\\\\\\"scr_acptcnt\\\\\\\":(.+?),";
    QList<QStringList> strLL = RegularExpression::match(pattern, html);
    strLL = RegularExpression::simplified(strLL);
    qDebug() << strLL;

    foreach (QStringList strList, strLL) {
        if(strList.size() == 11){
            QString electiveCode = strList.at(0);
            QString courseCode = _split(strList.at(1), " ", 0);
            QString courseName = _split(strList.at(1), " ", 1);
            QString credit = strList.at(2);
            QString isRequired = strList.at(3);
            QString hasMidtermExam = strList.at(4);
            QString hasFinalExam = strList.at(5);
            QString hasBefore = strList.at(6);
            QString courseClass = strList.at(7);
            QString timeClass = _split(strList.at(8), " ", -12, 1);
            QString teacher = _split(strList.at(8), " ", -2);
            QString totalOfPeople = strList.at(9);
            QString numberOfPeople = strList.at(10);

            Course course;
            course.set_electiveCode(electiveCode.toInt());
            course.set_courseName(courseName);
            course.set_credit(credit);
            course.set_isRequired(isRequired);
            course.set_hasMidtermExam(hasMidtermExam);
            course.set_hasFinalExam(hasFinalExam);
            course.set_courseClass(courseClass);
            course.set_teacher(teacher);
            course.set_timeClass(timeClass);
            course.set_totalOfPeople(totalOfPeople);
            course.set_numberOfPeople(numberOfPeople);
            course.set_teacher(teacher);
            course.set_courseCode(courseCode);
            course.set_year(course.year());
            course.set_semester(course.semester());

            courseList.append(course);
        }
    }

    return courseList;
}

double CourseSearchAnalyzer::coursePopularity(QString electiveCode)
{
    int year = 105;
    int semester = 2;
    const int countNumber = 8;

    Course course = courseNameFromElectiveCode(QString::number(year), QString::number(semester), electiveCode);
    double totalNumber = 0;
    double realNumber = 0;
    for(int i=0;i<countNumber;i++){
        --semester;
        if(semester == 0){
            semester = 2;
            --year;
        }
        course.set_year(QString::number(year));
        course.set_semester(QString::number(semester));

        QList<Course> courseList = courseListFromCourse(course);
        foreach (Course course_n, courseList) {
            totalNumber += course_n.totalOfPeople().toDouble();
            realNumber += course_n.numberOfPeople().toDouble();
            //qDebug() << course.numberOfPeople().toDouble() << course.totalOfPeople().toDouble();
        }
    }
    qDebug() << realNumber << totalNumber;

    if(totalNumber==0)
        return 0;
    return realNumber/totalNumber;
}
