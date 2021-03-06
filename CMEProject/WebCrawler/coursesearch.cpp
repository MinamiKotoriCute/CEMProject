#include "coursesearch.h"

#include <QFile>
#include <QLabel>
#include <QDebug>
#include <QTextCodec>
#include <QUrl>

#include "webautomaticindexer.h"
#include "regularexpression.h"
#include "expansion.h"

QString read(QString fileName){
    QString str;
    QFile f(fileName);
    if(f.open(QFile::ReadOnly)){
        str = QString::fromLocal8Bit(f.readAll());
        f.close();
    }
    return str;
}

void save(QByteArray b){
    static int i = 0;
    QFile f(QString::asprintf("%d.txt", ++i));
    if(f.open(QFile::WriteOnly)){
        f.write(b);
        f.close();
    }
}

QString split(QString source, QString sep, int number, int min = -1){
    QStringList strList = source.split(sep);
    if(number == -1){
        //find first
        if(strList.size() >= min)
            return strList.first();
    }
    else if(number == -2){
        //find end
        if(strList.size() >= min)
            return strList.last();
    }
    else if(strList.size() > number){
        return strList.at(number);
    }
    return QString();
}

CourseSearch::CourseSearch(QObject *parent) :
    QObject(parent),
    //codec(QTextCodec::codecForName("BIG5")),
    codec(QTextCodec::codecForName("UTF-8")),
    web(new WebAutomaticIndexer(this))
{
    /*QByteArray b = _read("A1.txt");

    QString pattern = "http-equiv=\"refresh\".+?url=(.+?)\"";
    //QString pattern = "service(\\d+?)\\.sds";
    QList<QStringList> strLL = RegularExpression::match(pattern, codec->toUnicode(b));
    //emit print("Url: " + web->getLastUrl());

    if(!strLL.isEmpty()){
        QStringList strList = strLL.first();
        if(!strList.isEmpty()){
            //serverNumber = strList.first();
            qDebug() << strList.first();
        }
    }*/

    //home();
    //courseFromName("105", "1", QStringLiteral("日本"));
    //courseFromWeek("105", "2", "1");
}

CourseSearch::~CourseSearch()
{
}

QString CourseSearch::home()
{
    //http://sdsweb.oit.fcu.edu.tw/coursequest/
    //http://service005.sds.fcu.edu.tw/FCU_director/director?entrance=coursequest
    //http://service001.sds.fcu.edu.tw/coursequest
    //http://service001.sds.fcu.edu.tw/coursequest/
    //QString url = "http://service001.sds.fcu.edu.tw/coursequest/";
    QString url = "http://sdsweb.oit.fcu.edu.tw/coursequest/";
    QByteArray html = web->get(url, 30000);
    //_save(html);

    QString pattern = "http-equiv=\"refresh\".+?url=(.+?)\"";
    QList<QStringList> strLL = RegularExpression::match(pattern, codec->toUnicode(html));
    //emit print("Url: " + web->getLastUrl());

    if(!strLL.isEmpty()){
        if(!strLL.first().isEmpty()){
            QString realUrl = strLL.first().first();
            homeUrl = realUrl;

            QString pattern = "service(\\d+?)\\.sds";
            QList<QStringList> strLL = RegularExpression::match(pattern, realUrl);
            if(!strLL.isEmpty())
                if(!strLL.first().isEmpty())
                    serverNumber = strLL.first().first();
            qDebug() << serverNumber << realUrl;

            QByteArray html = web->get(realUrl, 30000);
            //_save(html);
        }
    }

    //qDebug() << serverNumber << web->getLastUrl();
    return codec->toUnicode(html);
}

QString CourseSearch::courseFromName(QString year, QString semester, QString courseName)
{
    QString url = "http://service" + serverNumber + ".sds.fcu.edu.tw/Service/Search.asmx/GetType2Result";

    QStringList header;
    header << "Accept" << "application/json,text/plain,*/*";
    //header << "Cache-Control" << "max-age=0";
    header << "Connection" << "keep-alive";
    header << "Content-Type" << "application/json;charset=UTF-8";
    header << "Referer" << homeUrl;
    header << "User-Agent" << "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36";

    QString postData =  "{\"baseOptions\":{"
                        "\"lang\":\"cht\","
                        "\"year\":" + year + ","
                        "\"sms\":" + semester + "},"
                    "\"typeOptions\":{"
                        "\"code\":{"
                            "\"enabled\":false,"
                            "\"value\":\"\"},"
                        "\"weekPeriod\":{"
                            "\"enabled\":false,"
                            "\"week\":\"*\","
                            "\"period\":\"*\"},"
                        "\"course\":{"
                            "\"enabled\":true,"
                            "\"value\":\"" + courseName + "\"},"
                        "\"teacher\":{"
                            "\"enabled\":false,"
                            "\"value\":\"\"},"
                        "\"useEnglish\":{"
                            "\"enabled\":false},"
                        "\"specificSubject\":{"
                            "\"enabled\":false,"
                            "\"value\":\"1\"}}}";
    //qDebug() << codec->fromUnicode(postData);
    QByteArray html = web->post(url, codec->fromUnicode(postData), header);
    //_save(html);

    return codec->toUnicode(html);
}

QString CourseSearch::courseFromNameTeacher(QString year, QString semester, QString courseName, QString teacher)
{
    QString url = "http://service" + serverNumber + ".sds.fcu.edu.tw/Service/Search.asmx/GetType2Result";

    QStringList header;
    header << "Accept" << "application/json,text/plain,*/*";
    //header << "Cache-Control" << "max-age=0";
    header << "Connection" << "keep-alive";
    header << "Content-Type" << "application/json;charset=UTF-8";
    header << "Referer" << homeUrl;
    header << "User-Agent" << "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36";

    QString postData =  "{\"baseOptions\":{"
                        "\"lang\":\"cht\","
                        "\"year\":" + year + ","
                        "\"sms\":" + semester + "},"
                    "\"typeOptions\":{"
                        "\"code\":{"
                            "\"enabled\":false,"
                            "\"value\":\"\"},"
                        "\"weekPeriod\":{"
                            "\"enabled\":false,"
                            "\"week\":\"*\","
                            "\"period\":\"*\"},"
                        "\"course\":{"
                            "\"enabled\":true,"
                            "\"value\":\"" + courseName + "\"},"
                        "\"teacher\":{"
                            "\"enabled\":true,"
                            "\"value\":\"" + teacher + "\"},"
                        "\"useEnglish\":{"
                            "\"enabled\":false},"
                        "\"specificSubject\":{"
                            "\"enabled\":false,"
                            "\"value\":\"1\"}}}";
    //qDebug() << codec->fromUnicode(postData);
    QByteArray html = web->post(url, codec->fromUnicode(postData), header);
    _save(html);

    return codec->toUnicode(html);
}

QString CourseSearch::courseFromWeek(QString year, QString semester, QString week)
{
    QString url = "http://service" + serverNumber + ".sds.fcu.edu.tw/Service/Search.asmx/GetType2Result";

    QStringList header;
    header << "Accept" << "application/json,text/plain,*/*";
    //header << "Cache-Control" << "max-age=0";
    header << "Connection" << "keep-alive";
    header << "Content-Type" << "application/json;charset=UTF-8";
    header << "Referer" << homeUrl;
    header << "User-Agent" << "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36";

    QString postData =  "{\"baseOptions\":{"
                        "\"lang\":\"cht\","
                        "\"year\":" + year + ","
                        "\"sms\":" + semester + "},"
                    "\"typeOptions\":{"
                        "\"code\":{"
                            "\"enabled\":false,"
                            "\"value\":\"\"},"
                        "\"weekPeriod\":{"
                            "\"enabled\":true,"
                            "\"week\":\"%1\","
                            "\"period\":\"" + week + "\"},"
                        "\"course\":{"
                            "\"enabled\":false,"
                            "\"value\":\"\"},"
                        "\"teacher\":{"
                            "\"enabled\":false,"
                            "\"value\":\"\"},"
                        "\"useEnglish\":{"
                            "\"enabled\":false},"
                        "\"specificSubject\":{"
                            "\"enabled\":false,"
                            "\"value\":\"1\"}}}";

    QByteArray html = web->post(url, codec->fromUnicode(postData.arg("*")), header);
    if(web->getLastStatusCode()>=200 && web->getLastStatusCode()<300){
        //seccess
        return codec->toUnicode(html);
    }
    //_save(html);

    return codec->toUnicode(html);
}

QString CourseSearch::courseFromElectiveCode(QString year, QString semester, QString electiveCode)
{
    QString url = "http://service" + serverNumber + ".sds.fcu.edu.tw/Service/Search.asmx/GetType2Result";

    QStringList header;
    header << "Accept" << "application/json, text/plain, */*";
    //header << "Cache-Control" << "max-age=0";
    header << "Connection" << "keep-alive";
    header << "Content-Type" << "application/json; charset=UTF-8";
    header << "Referer" << homeUrl;
    header << "User-Agent" << "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36";

    QString postData =  "{\"baseOptions\":{"
                            "\"lang\":\"cht\","
                            "\"year\":" + year + ","
                            "\"sms\":" + semester + "},"
                        "\"typeOptions\":{"
                            "\"code\":{"
                                "\"enabled\":true,"
                                "\"value\":\"" + electiveCode + "\"},"
                            "\"weekPeriod\":{"
                                "\"enabled\":false,"
                                "\"week\":\"*\","
                                "\"period\":\"*\"},"
                            "\"course\":{"
                                "\"enabled\":false,"
                                "\"value\":\"\"},"
                            "\"teacher\":{"
                                "\"enabled\":false,"
                                "\"value\":\"\"},"
                            "\"useEnglish\":{"
                                "\"enabled\":false},"
                            "\"specificSubject\":{"
                                "\"enabled\":false,"
                                "\"value\":\"1\"}}}";

    QByteArray html = web->post(url, codec->fromUnicode(postData), header);
    _save(html);

    return codec->toUnicode(html);
}

QString CourseSearch::login(QString username, QString password)
{
    QString url = "http://service" + serverNumber + ".sds.fcu.edu.tw/Service/Auth.asmx/login";

    QStringList header;
    header << "Accept" << "application/json,text/plain,*/*";
    header << "Connection" << "keep-alive";
    header << "Content-Type" << "application/json;charset=UTF-8";
    header << "Referer" << homeUrl;
    header << "User-Agent" << "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36";

    QString postData =  "{\"id\":\"" + username + "\","
                        "\"password\":\"" + password + "\","
                        "\"baseOptions\":{"
                            "\"lang\":\"cht\","
                            "\"year\":105,"
                            "\"sms\":2}}";

    QByteArray html = web->post(url, codec->fromUnicode(postData), header);
    _save(html);

    return codec->toUnicode(html);
}

QString CourseSearch::electiveCoursesList(QString year, QString semester)
{
    QString url = "http://service" + serverNumber +  ".sds.fcu.edu.tw/coursequest/student/courselistbystudent.jsp";

    QStringList header;
    header << "Accept" << "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8";
    header << "Cache-Control" << "max-age=0";
    header << "Connection" << "keep-alive";
    header << "Referer" << "http://service" + serverNumber +  ".sds.fcu.edu.tw/coursequest/condition.jsp";
    header << "User-Agent" << "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36";

    QStringList postList;
    postList << "text2" << "";
    postList << "yms_year" << year;
    postList << "yms_smester" << semester;
    postList << "item" << "9";

    QByteArray postData = WebAutomaticIndexer::toHtmlData(postList);

    QByteArray html = web->post(url, postData, header);
    //save(html);

    //QString pattern = "<TD.+?\">(.+?)</a></TD>.+?align=center>(.+?)</TD>.+?align=center>(.+?)</TD>.+?align=center>(.+?)</TD>";
    /*QString pattern = "<TR class=row1><TD  align=center>(?:.+?)</TD>.+?href=\"(.+?)\">(.+?)</a>.+?align=center>(.+?)</TD>.+?align=center>(.+?)</TD>.+?align=center>(.+?)</TD>.+?align=center>(.+?)</TD>.+?align=center>(.+?)</TD>.+?</TR>";
    QList<QStringList> strLL = RegularExpression::match(pattern, QString::fromLocal8Bit(html));
    strLL = RegularExpression::simplified(strLL);*/

    //qDebug() << strLL;
    return codec->toUnicode(html);
}

QString CourseSearch::coursesList(QString year, QString semester, QString item, QString text)
{
    QString url = "http://service" + serverNumber +  ".sds.fcu.edu.tw/coursequest/course/courselist.jsp";

    QStringList header;
    header << "Accept" << "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8";
    header << "Cache-Control" << "max-age=0";
    header << "Connection" << "keep-alive";
    header << "Referer" << "http://service" + serverNumber +  ".sds.fcu.edu.tw/coursequest/condition.jsp";
    header << "User-Agent" << "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36";

    QStringList postList;
    postList << "text2" << "";
    postList << "yms_year" << year;
    postList << "yms_smester" << semester;
    postList << "item" << item;

    QByteArray postData = WebAutomaticIndexer::toHtmlData(postList);
    postData += "&condition_key=" + text.toLocal8Bit();

    QByteArray html = web->post(url, postData, header);
    return codec->toUnicode(html);
    //save(html);
/*
    QString pattern = "<TD width=50 >(.+?)</TD>.+?href=\"(.+?)\">(.+?)</A>.+?<TD >(\\d+?)</TD>.+?<TD >(.+?)</TD>.+?<TD >(.+?)</TD>.+?<TD >(.+?)</TD>.+?<TD >(.+?)</TD>.+?\">(.+?)</A>.+?<TD >(.+?)</TD>.+?<TD >(.+?)</TD>.+?<TD >";
    //("0923", "coursesummary.jsp?strCourseId=1051CB2613199212001MKT344aCB26139", "MKT344 日本商情與策略", "3", "選修", "否", "否", "否", "行銷三甲", "(二)02-04 人607 黃煇慶", "60/31")
    QList<QStringList> strLL = RegularExpression::match(pattern, QString::fromLocal8Bit(html));
    strLL = RegularExpression::simplified(strLL);

    QList<QStringList> targetStrLL;
    foreach (QStringList strList, strLL) {
        if(strList.size() < 8)
            continue;
        if(split(strList.at(8), " ", -2) == teacherName){
            targetStrLL << strList;
        }
    }

    if(electiveCode.isEmpty()){
        return targetStrLL;
    }

    QList<QStringList> targetStrLL2;
    foreach (QStringList strList, targetStrLL) {
        if(strList.isEmpty())
            continue;
        if(strList.at(0) == electiveCode){
            targetStrLL2 << strList;
            break;
        }
    }

    return targetStrLL2;*/
}

/*
QList<QStringList> CourseSearch::electiveCoursesAnalysis()
{
    int year = 105;
    int semester = 1;
    QList<QStringList> retValue;

    QList<QStringList> courseStrLL = electiveCoursesList(QString::number(year), QString::number(semester));
    //everyone class of now
    foreach(QStringList strList, courseStrLL){
        if(strList.size() < 3)
            continue;
        QStringList courseInformationList = static_cast<QString>(strList.at(0)).split(" ");
        if(courseInformationList.size() < 2)
            continue;
        QString className = courseInformationList.at(1);
        QString teacherName = split(strList.at(2), " ", -2, 3);
        if(teacherName.isEmpty())
            continue;
        QString electiveCode = strList.at(3);

        //calculate 8 semester
        int totalPeople = 0;
        int remainPeople = 0;
        for(int i=0;i<7;i++){
            QList<QStringList> targetCourse = coursesList(QString::number(year-(i+1)/2), QString::number(semester+(i%2)), "2", className, teacherName);
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
        retValue << courseTotalInformation;

        qDebug() << className << teacherName << totalPeople << remainPeople;
    }
    //qDebug() << courseStrLL;
    return retValue;
}*/
