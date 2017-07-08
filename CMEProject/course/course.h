#ifndef COURSE_H
#define COURSE_H

#include <QString>

//#include "defineexpansion.h"

#define DATACLASS_H(name)\
public:\
    name();\
    name(const name &other);\
    void operator=(const name &other);\
    ~name();\
    bool isNull();\
    void* address(){return d;}\
    int count();\
private:\
    class name##Private;\
    name##Private *d;

#define DATACLASS_CPP(name)\
    name::name() : d(nullptr){}\
    name::name(const name &other){\
        d = other.d;\
        if(d != nullptr) d->reference();}\
    void name::operator=(const name &other){\
        d = other.d;\
        if(d != nullptr) d->reference();}\
    name::~name(){\
        if(d != nullptr) d->free();}\
    bool name::isNull(){return d==nullptr;}\
    int name::count(){\
        if(d != nullptr) return d->count();\
        return 0;}

#define DATAFUNTION_H(type, name)\
public:\
    void set_##name(type name);\
    type name();

#define DATAFUNTION_CPP(className, type, name)\
    void className::set_##name(type name){\
        if(d == nullptr) d = new className##Private;\
        else if(d->count() > 1){\
            d->free();\
            d = d->clone();}\
        d->set_##name(name);}\
    type className::name(){\
        if(d == nullptr) d = new className##Private;\
        return d->name();}


class Course
{
    DATACLASS_H(Course)

    DATAFUNTION_H(int, id)
    DATAFUNTION_H(int, electiveCode)
    DATAFUNTION_H(QString, courseName)
    DATAFUNTION_H(QString, courseCode)
    DATAFUNTION_H(QString, credit)
    DATAFUNTION_H(QString, isRequired)
    DATAFUNTION_H(QString, hasMidtermExam)
    DATAFUNTION_H(QString, hasFinalExam)
    DATAFUNTION_H(QString, teacher)
    DATAFUNTION_H(QString, numberOfPeople)
    DATAFUNTION_H(QString, totalOfPeople)
    DATAFUNTION_H(QString, timeClass)
    DATAFUNTION_H(QString, courseClass)
    DATAFUNTION_H(QString, year)
    DATAFUNTION_H(QString, semester)

/*
    SETGETFUNTION(electiveCode, int)
    SETGETFUNTION(courseName, QString)
    SETGETFUNTION(credit, QString)
    SETGETFUNTION(isRequired, QString)
    SETGETFUNTION(hasMidtermExam, QString)
    SETGETFUNTION(hasFinalExam, QString)
    SETGETFUNTION(detail, QString)
    SETGETFUNTION(numberOfPeople, QString)*/
};

#endif // COURSE_H
