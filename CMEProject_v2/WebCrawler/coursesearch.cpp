#include "coursesearch.h"

#include <QTextCodec>
#include <QDebug>
#include <QJsonDocument>
#include <QRegularExpression>

#include "webautomaticindexer.h"
#include "regularexpression.h"
#include "expansion.h"

CourseSearch::CourseSearch(QObject *parent) :
    QObject(parent),
    codec(QTextCodec::codecForName("UTF-8")),
    web(new WebAutomaticIndexer(this))
{

}

bool CourseSearch::home()
{
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
            if(!strLL.isEmpty()){
                if(!strLL.first().isEmpty()){
                    serverNumber = strLL.first().first();
                    emit print("CourseSearch::home() get serverNumber: " + serverNumber);
                }
            }
            //qDebug() << serverNumber << realUrl;

            QByteArray html = web->get(realUrl, 30000);
            //_save(html);
            return true;
        }
    }

    //qDebug() << serverNumber << web->getLastUrl();
    return false;
}

bool CourseSearch::login(QString username, QString password)
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
    //_save(html);

    QString html_u = codec->toUnicode(html);
    //qDebug() << html_u;
    if(html_u.contains("id")){
        qDebug() << "login seccess";
        return true;
    }
    else{
        qDebug() << "login fail";
    }

    return false;
}

QList<QJsonObject> CourseSearch::getAllCourse(QString year, QString semester)
{
    return QList<QJsonObject>();
}

QList<QJsonObject> CourseSearch::courseFromWeek(QString year, QString semester, QString week, QString time)
{
    QList<QJsonObject> retValue;
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
                            "\"week\":\"" + week + "\","
                            "\"period\":\"" + time + "\"},"
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
    //qDebug() << "html size" << html.size();
    //qDebug() << "html_u size" << codec->toUnicode(html).size();
    //_save(html);

    //qDebug() << "web->getLastStatusCode()" << web->getLastStatusCode();

    if(web->getLastStatusCode()>=200 && web->getLastStatusCode()<300){
        //seccess
        /*QString pattern = "{"
                          "\\\\\\\"scr_selcode\\\\\\\":\\\\\\\"1274\\\\\\\","
                          "\\\\\\\"sub_name\\\\\\\":\\\\\\\"FCME104 計算機程式－VB\\\\\\\","
                          "\\\\\\\"scr_credit\\\\\\\":2.0,"
                          "\\\\\\\"scj_scr_mso\\\\\\\":\\\\\\\"必修\\\\\\\","
                          "\\\\\\\"scr_examid\\\\\\\":\\\\\\\"是\\\\\\\","
                          "\\\\\\\"scr_examfn\\\\\\\":\\\\\\\"是\\\\\\\","
                          "\\\\\\\"scr_exambf\\\\\\\":\\\\\\\"否\\\\\\\","
                          "\\\\\\\"cls_name\\\\\\\":\\\\\\\"纖複一甲\\\\\\\","
                          "\\\\\\\"scr_period\\\\\\\":\\\\\\\"(四)01     忠303 (四)02     忠611(電腦實習室) 李維平\\\\\\\","
                          "\\\\\\\"scr_precnt\\\\\\\":65.0,"
                          "\\\\\\\"scr_acptcnt\\\\\\\":0.0,"
                          "\\\\\\\"scr_remarks\\\\\\\":\\\\\\\" \\\\\\\","
                          "\\\\\\\"unt_ls\\\\\\\":4008,"
                          "\\\\\\\"cls_id\\\\\\\":\\\\\\\"CE05111\\\\\\\","
                          "\\\\\\\"sub_id\\\\\\\":\\\\\\\"26390\\\\\\\","
                          "\\\\\\\"scr_dup\\\\\\\":\\\\\\\"001\\\\\\\"}";*/

        //("1785", "GBEN101 專業外語", "2.0", "必修", "否", "否", "否", "自控三甲", "(三)01-02 人401 斐克禮", "65.0", "0.0", "\\\"非本班同學，請勿選課。\\\"", "4504", "CE12131", "43034", "001")
        QString pattern = "{"
                          "\\\\\\\"scr_selcode\\\\\\\":\\\\\\\"(.*?)\\\\\\\","
                          "\\\\\\\"sub_name\\\\\\\":\\\\\\\"(.*?)\\\\\\\","
                          "\\\\\\\"scr_credit\\\\\\\":(.*?),"
                          "\\\\\\\"scj_scr_mso\\\\\\\":\\\\\\\"(.*?)\\\\\\\","
                          "\\\\\\\"scr_examid\\\\\\\":\\\\\\\"(.*?)\\\\\\\","
                          "\\\\\\\"scr_examfn\\\\\\\":\\\\\\\"(.*?)\\\\\\\","
                          "\\\\\\\"scr_exambf\\\\\\\":\\\\\\\"(.*?)\\\\\\\","
                          "\\\\\\\"cls_name\\\\\\\":\\\\\\\"(.*?)\\\\\\\","
                          "\\\\\\\"scr_period\\\\\\\":\\\\\\\"(.*?)\\\\\\\","
                          "\\\\\\\"scr_precnt\\\\\\\":(.*?),"
                          "\\\\\\\"scr_acptcnt\\\\\\\":(.*?),"
                          "\\\\\\\"scr_remarks\\\\\\\":(.*?),"
                          "\\\\\\\"unt_ls\\\\\\\":(.*?),"
                          "\\\\\\\"cls_id\\\\\\\":\\\\\\\"(.*?)\\\\\\\","
                          "\\\\\\\"sub_id\\\\\\\":\\\\\\\"(.*?)\\\\\\\","
                          "\\\\\\\"scr_dup\\\\\\\":\\\\\\\"(.*?)\\\\\\\"}";
        //QString pattern = "{\\\\\\\"scr_selcode\\\\\\\":\\\\\\\"(.*?)\\\\\\\",";

        QRegularExpression mainRE(pattern);
        mainRE.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);

        QRegularExpressionMatchIterator mainIt = mainRE.globalMatch(codec->toUnicode(html));
        while(mainIt.hasNext()){
            QRegularExpressionMatch mainMatch = mainIt.next();
            QStringList strList = mainMatch.capturedTexts();
            if(!strList.isEmpty())
                strList.removeFirst();

            //qDebug() << strList;
            if(strList.size() == 16){
                QJsonObject course;
                course["electiveCode"] = strList.at(0);
                course["courseCode"] = _split(strList.at(1), " ", 0);
                course["courseName"] = _split(strList.at(1), " ", 1);
                course["credit"] = strList.at(2);
                course["required"] = strList.at(3);
                course["midTest"] = strList.at(4);
                course["finalTest"] = strList.at(5);
                course["beforeTest"] = strList.at(6);
                course["courseClass"] = strList.at(7);
                course["time"] = _split(strList.at(8), " ", -12, 1);
                course["teacher"] = _split(strList.at(8), " ", -2);
                course["defaultNumberOfPeople"] = strList.at(9);
                course["lastNumberOfPeople"] = strList.at(10);
                QString remark = strList.at(11);
                //qDebug() << remark;
                if(remark != "null"){
                    if(remark.size() >= 4){
                        if(remark.at(0) == "\\"){
                            remark.remove(0,2);
                            remark.remove(remark.size()-2,2);
                        }
                    }
                    course["remark"] = remark;
                }

                course["year"] = year;
                course["semester"] = semester;

                retValue << course;
                //qDebug() << course;
            }
        }
    }
    else if(time == "*"){
        for(int i=0;i<15;i++){
            retValue << courseFromWeek(year, semester, week, QString::number(i));
        }
    }

    return retValue;
}

QString CourseSearch::getServerNumber()
{
    return serverNumber;
}
