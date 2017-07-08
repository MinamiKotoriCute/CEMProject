#ifndef COURSEPRIVATE_H
#define COURSEPRIVATE_H

#include "course.h"

#define PRIVATEDATACLASS(name)\
public:\
    void reference(){++_count;}\
    void free(){\
        --_count;\
        if(_count == 0) delete this;}\
    int count(){return _count;}\
private:\
    int _count;

#define PRIVATEDATEFUNTION(type, name)\
public:\
    void set_##name(type name){_##name = name;}\
    type name(){return _##name;}\
private:\
    type _##name;

class Course::CoursePrivate
{
public:
    CoursePrivate();
    CoursePrivate *clone();
    PRIVATEDATACLASS(Course)

    PRIVATEDATEFUNTION(int, id)
    PRIVATEDATEFUNTION(int, electiveCode)
    PRIVATEDATEFUNTION(QString, courseName)
    PRIVATEDATEFUNTION(QString, courseCode)
    PRIVATEDATEFUNTION(QString, credit)
    PRIVATEDATEFUNTION(QString, isRequired)
    PRIVATEDATEFUNTION(QString, hasMidtermExam)
    PRIVATEDATEFUNTION(QString, hasFinalExam)
    PRIVATEDATEFUNTION(QString, teacher)
    PRIVATEDATEFUNTION(QString, numberOfPeople)
    PRIVATEDATEFUNTION(QString, totalOfPeople)
    PRIVATEDATEFUNTION(QString, timeClass)
    PRIVATEDATEFUNTION(QString, courseClass)
    PRIVATEDATEFUNTION(QString, year)
    PRIVATEDATEFUNTION(QString, semester)

    /*SETGETFUNTION(electiveCode, int)
    SETGETFUNTION(courseName, QString)
    SETGETFUNTION(credit, QString)
    SETGETFUNTION(isRequired, QString)
    SETGETFUNTION(hasMidtermExam, QString)
    SETGETFUNTION(hasFinalExam, QString)
    SETGETFUNTION(detail, QString)
    SETGETFUNTION(numberOfPeople, QString)*/
};

#endif // COURSEPRIVATE_H
