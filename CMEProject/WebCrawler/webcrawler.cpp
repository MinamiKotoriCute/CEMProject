#include "webcrawler.h"

#include "coursesearch.h"

WebCrawler::WebCrawler(QString username, QString password, QObject *parent) :
    QObject(parent),
    username(username),
    password(password)
{
    //CourseSearch c(username, password);
}
