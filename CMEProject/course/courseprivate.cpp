#include "courseprivate.h"

#include <QDebug>
/*
Course::CoursePrivate::CoursePrivate() : count(1)
{
    qDebug() << "count" << count;
}

void Course::CoursePrivate::reference()
{
    ++count;
    qDebug() << "count" << count;
}

void Course::CoursePrivate::free()
{
    --count;
    qDebug() << "count" << count;
    if(count == 0){
        qDebug() << "delete";
        delete this;
    }
}
*/

Course::CoursePrivate::CoursePrivate() :
    _count(1)
{
    _electiveCode = 0;
}

Course::CoursePrivate *Course::CoursePrivate::clone()
{
    CoursePrivate *c = new CoursePrivate;
    c->set_courseClass(_courseClass);
    c->set_courseName(_courseName);
    c->set_courseCode(_courseCode);
    c->set_credit(_credit);
    c->set_electiveCode(_electiveCode);
    c->set_hasFinalExam(_hasFinalExam);
    c->set_hasMidtermExam(_hasMidtermExam);
    c->set_id(_id);
    c->set_isRequired(_isRequired);
    c->set_numberOfPeople(_numberOfPeople);
    c->set_teacher(_teacher);
    c->set_timeClass(_timeClass);
    c->set_totalOfPeople(_totalOfPeople);
    c->set_year(_year);
    c->set_semester(_semester);
    return c;
}
