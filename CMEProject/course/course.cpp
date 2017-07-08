#include "course.h"

#include "courseprivate.h"


DATACLASS_CPP(Course)
DATAFUNTION_CPP(Course, int, id)
DATAFUNTION_CPP(Course, int, electiveCode)
DATAFUNTION_CPP(Course, QString, courseName)
DATAFUNTION_CPP(Course, QString, courseCode)
DATAFUNTION_CPP(Course, QString, credit)
DATAFUNTION_CPP(Course, QString, isRequired)
DATAFUNTION_CPP(Course, QString, hasMidtermExam)
DATAFUNTION_CPP(Course, QString, hasFinalExam)
DATAFUNTION_CPP(Course, QString, teacher)
DATAFUNTION_CPP(Course, QString, numberOfPeople)
DATAFUNTION_CPP(Course, QString, totalOfPeople)
DATAFUNTION_CPP(Course, QString, timeClass)
DATAFUNTION_CPP(Course, QString, courseClass)
DATAFUNTION_CPP(Course, QString, year)
DATAFUNTION_CPP(Course, QString, semester)


/*
Course::Course() : d(nullptr)
{

}

Course::Course(const Course &other)
{
    d = other.d;
    if(d != nullptr)
        d->reference();
}

Course::~Course()
{
    if(d != nullptr)
        d->free();
}

void Course::operator=(const Course &other)
{
    d = other.d;
    if(d != nullptr)
        d->reference();
}

bool Course::isNull()
{
    return d == nullptr;
}
*/
