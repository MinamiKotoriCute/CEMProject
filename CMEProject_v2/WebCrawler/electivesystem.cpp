#include "electivesystem.h"

#include <QTextCodec>
#include <QDebug>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QImage>
#include <QLabel>

#include "webautomaticindexer.h"
#include "regularexpression.h"
#include "expansion.h"

ElectiveSystem::ElectiveSystem(QObject *parent) :
    QObject(parent),
    codec(QTextCodec::codecForName("UTF-8")),
    web(new WebAutomaticIndexer(this))
{
    connect(web, &WebAutomaticIndexer::print, this, [=](QString str){
        qDebug() << str;
    });

    QByteArray html = _read("2.txt");
    QStringList postData = findPostDataValue(html);
}

bool ElectiveSystem::home()
{
    QString url = "http://course.fcu.edu.tw/course/";
    firstHtml = web->get(url, 30000);
    _save(firstHtml);

    QByteArray html2 = web->get("https://course.fcu.edu.tw/validateCode.aspx", 30000);

    QList<QByteArray> headerList = web->getLastReplyHeaderList();
    QList<QByteArray> headerValueList = web->getLastReplyHeaderValueList();
    foreach(QByteArray head, headerList) {
        if(head == "Set-Cookie"){
            QByteArray str = headerValueList.at(headerList.indexOf(head));

            QString pattern = "=(.+?);";
            QList<QStringList> strLL = RegularExpression::match(pattern, codec->toUnicode(str));

            if(!strLL.isEmpty()){
                if(!strLL.first().isEmpty()){
                    checkCode = strLL.first().first();
                }
            }
        }
    }
    return true;
}

bool ElectiveSystem::login(QString username, QString password)
{
    QString url = "https://course.fcu.edu.tw/Login.aspx";
    QStringList postData = findPostDataValue(firstHtml);
    for(int i=0;i<postData.size();i+=2){
        if(postData.at(i) == "__EVENTTARGET"){
            if(postData.size() > i+1){
                postData.removeAt(i+1);
                postData.insert(i+1, "ctl00$Login1$LoginButton");
                break;
            }
        }
    }
    postData << "ctl00$Login1$RadioButtonList1" << "zh-tw";
    postData << "ctl00$Login1$UserName" << username;
    postData << "ctl00$Login1$Password" << password;
    postData << "ctl00$Login1$vcode" << checkCode;
    postData << "ctl00$temp" << "";

    QStringList header;
    header << "Accept" << "*/*";
    header << "Cache-Control" << "max-age=0";
    header << "Connection" << "keep-alive";
    //header << "Content-Type" << "charset=UTF-8";
    header << "Origin" << "https://course.fcu.edu.tw";
    header << "Referer" << "https://course.fcu.edu.tw/Login.aspx";
    header << "User-Agent" << "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36";

    QByteArray html = web->post(url, WebAutomaticIndexer::toHtmlData(postData), header);
    _save(html);
    mainUrl = web->getLastUrl();

    QRegularExpression mainRE("<td class=\"week([ sw]*?)\"><a.+?class=\"tooltip\" data-title=\"(.+?)\".*?>(.+?)</a></td>");
    QRegularExpressionMatchIterator mainIt = mainRE.globalMatch(html);

    while(mainIt.hasNext()){
        QRegularExpressionMatch mainMatch = mainIt.next();
        QString type = mainMatch.captured(1);
        QString courseName = mainMatch.captured(2);
        QString electiveCode = mainMatch.captured(3);

        if(type == " s")
            qDebug() << "choose" << courseName << electiveCode;
        else if(type == " w")
            qDebug() << "want" << courseName << electiveCode;
        else
            qDebug() << courseName << electiveCode;
    }

    return true;
}

void ElectiveSystem::search(QString electiveCode)
{
    QString url = mainUrl;
}

QStringList ElectiveSystem::findPostDataValue(const QString &source)
{
    QStringList retValue;

    QRegularExpression mainRE("<input type=\"\\S*?\" name=\"(\\S+?)\" id=\"(\\S+?)\" value=\"(.*?)\" />");
    QRegularExpressionMatchIterator mainIt = mainRE.globalMatch(source);

    while(mainIt.hasNext()){
        QRegularExpressionMatch mainMatch = mainIt.next();
        QString name = mainMatch.captured(1);
        QString id = mainMatch.captured(2);
        QString value = mainMatch.captured(3);

        qDebug() << name << value;
        retValue << name << value;
    }

    return retValue;
}
