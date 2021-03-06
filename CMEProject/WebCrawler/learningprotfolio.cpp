#include "learningprotfolio.h"

#include <QTextCodec>
#include <QUrl>
#include <QMap>
#include <QRegularExpression>
#include <QTextEdit>
#include <QDebug>

#include "webautomaticindexer.h"
#include "regularexpression.h"
#include "expansion.h"
#include "database.h"

LearningProtfolio::LearningProtfolio(QObject *parent) :
    QObject(parent),
    codec(QTextCodec::codecForName("UTF-8")),
    web(new WebAutomaticIndexer(this))
{
    /*QByteArray b = _read("2.txt");

    QString pattern = "sub_name\">(.+?)</span>.+?sco_score\">(.+?)</span>.+?allcount\">(.+?)</span>.+?allavg\">(.+?)</span>.+?rankNum\">(.+?)</span>.+?<tr align=\"center\">(.+?)</tr>";
    QList<QStringList> strLL = RegularExpression::match(pattern, codec->toUnicode(b));
    if(strLL.size()==1){
        QStringList strList = strLL.first();
        if(strList.size()==6){
            QString className = strList.at(0);
            QString score = strList.at(1);
            QString allcount = strList.at(2);
            QString allavg = strList.at(3);
            QString rankNum = strList.at(4);
            QString distribution = strList.at(5);
            for(int i=0;i<strList.size()-1;i++)
                qDebug() << strList.at(i);

            pattern = "title='.+? (\\d+?) .+?' />";
            strLL = RegularExpression::match(pattern, distribution);
            foreach (QStringList strList, strLL) {
                qDebug() << strList;
            }
        }
    }*/
    //qDebug() << strLL.size() << strLL;

    //QTextEdit *t = new QTextEdit(codec->toUnicode(b));
    //t->show();

    home();
    login("D0250179", "joeyten2000123");
    test();
}

void LearningProtfolio::home()
{
    QString url = "http://learningportfolio.fcu.edu.tw/tw/index.aspx";
    lastHtml = web->get(url);
}

bool LearningProtfolio::login(QString username, QString password)
{
    QStringList postData = findLearningProtfolioValue(QString::fromUtf8(lastHtml));
    postData << "ctl00$txt_Search" << "";
    postData << "ctl00$ddlanguage" << "tw";
    postData << "ctl00$ucLogin$txtID" << username;
    postData << "ctl00$ucLogin$txtPW" << password;
    postData << "ctl00$ucLogin$btnLogin" << QUrl::fromPercentEncoding("%E7%99%BB%E5%85%A5");
    postData << "ctl00$MainContent$ViewFrom" << "";
    _save(web->post("http://learningportfolio.fcu.edu.tw/tw/index.aspx", WebAutomaticIndexer::toHtmlData(postData)));

    return 0;
}

void LearningProtfolio::test(int n)
{
    QByteArray firstHtml;
    if(n==-1)
        firstHtml = myTranscripts();
    else
        firstHtml = myTranscripts(n);

    QString pattern = "<input type=\"submit\" name=\"ctl00\\$MainContent(.+?)class=\"submit_button\" />";
    QList<QStringList> strLL = RegularExpression::match(pattern, codec->toUnicode(firstHtml));
    maxSemester = strLL.size();
    qDebug() << maxSemester;

    if(n==-1){
        lastHtml = firstHtml;
        for(int i=0;i<maxSemester;i++){
            test(i);
        }
    }
    else{
        pattern = "<tr id=\"ctl00_MainContent_LV.+?<td align=\"center\">(.+?)</td>.+?<a href='(.+?)&TB_iframe=true&height=450&width=600&modal=true";
        strLL = RegularExpression::match(pattern, codec->toUnicode(firstHtml));
        strLL = RegularExpression::simplified(strLL);
        foreach (QStringList strList, strLL) {
            //qDebug() << strList;
            QStringList insertList = courseTranscript(codec->fromUnicode(strList.at(1)));
            insertList << "CourseSearchCourse_courseCode" << strList.at(0);
            insertList << "year" << lastYear;
            insertList << "semester" << lastSemester;
            Database db;
            db.insertValue("CourseLearningProtfolie", insertList);
            //_save(courseTranscript(codec->fromUnicode(strLL.at(0).at(0))));
        }
    }
    //myTranscripts(0);
}

QStringList LearningProtfolio::courseTranscript(QByteArray url)
{
    url = "http://learningportfolio.fcu.edu.tw/tw/" + url + "&&undefined";
    QByteArray html = web->get(url);
    QStringList retList;

    QString pattern = "sub_name\">(.+?)</span>.+?sco_score\">(.+?)</span>.+?allcount\">(.+?)</span>.+?allavg\">(.+?)</span>.+?rankNum\">(.+?)</span>.+?<tr align=\"center\">(.+?)</tr>";
    QList<QStringList> strLL = RegularExpression::match(pattern, codec->toUnicode(html));
    if(strLL.size()==1){
        QStringList strList = strLL.first();
        if(strList.size()==6){
            QStringList insertList;
            QString className = strList.at(0);
            QString score = strList.at(1);
            QString allcount = strList.at(2);
            QString allavg = strList.at(3);
            QString rankNum = strList.at(4);
            QString distribution = strList.at(5);
            for(int i=0;i<strList.size()-1;i++)
                qDebug() << strList.at(i);

            pattern = "title='.+? (\\d+?) .+?' />";
            strLL = RegularExpression::match(pattern, distribution);
            int i = 0;
            foreach (QStringList strList, strLL) {
                //qDebug() << QString::asprintf("%d - %d: ",i, i+5) << strList;
                i+=5;
            }

            //strList << "CourseSearchCourse_courseCode" << "TEXT";
            retList << "numberOfPeople" << allcount;
            retList << "average" << allavg;
            for(int i=1;i<=20;i++)
                retList << QString::asprintf("numberAt%d", i*5) << strLL.at(i-1).first();
        }
    }

    //return html;
    return retList;
}

QByteArray LearningProtfolio::myTranscripts()
{
    QString url = "http://learningportfolio.fcu.edu.tw/tw/LearnAssist_Grades.aspx";
    QByteArray html = web->get(url);
    return html;
}

QByteArray LearningProtfolio::myTranscripts(int n)
{
    QString url = "http://learningportfolio.fcu.edu.tw/tw/LearnAssist_Grades.aspx";
    QStringList postData = findLearningProtfolioValue2(lastHtml, n);
    QStringList header;
    header << "Accept" << "*/*";
    header << "Cache-Control" << "no-cache";
    header << "Connection" << "keep-alive";
    //header << "Content-Type" << "charset=UTF-8";
    header << "Origin" << "http://learningportfolio.fcu.edu.tw";
    header << "Referer" << url;
    header << "User-Agent" << "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36";
    header << "X-MicrosoftAjax" << "Delta=true";
    QByteArray html = web->post(url, WebAutomaticIndexer::toHtmlData(postData), header);
    return html;
}

QStringList LearningProtfolio::findLearningProtfolioValue2(QByteArray lastHtml, int n)
{
    QMap<QString, QString> map;
    QString s1 = QString::asprintf("ctl00_MainContent_DL_Smester_ctl0%d_hf_yms_year", n);
    QString s2 = QString::asprintf("ctl00_MainContent_DL_Smester_ctl0%d_hf_yms_smester", n);
    map.insert(s1, "");
    map.insert(s2, "");

    QStringList postData;
    postData << "ctl00$ScriptManager1" << QString::asprintf("ctl00$MainContent$UpdatePanel1|ctl00$MainContent$DL_Smester$ctl0%d$btnSmester", n);
    postData << findLearningProtfolioValue(QString::fromUtf8(lastHtml), &map);
    postData << "ctl00$txt_Search" << "";
    postData << "ctl00$ddlanguage" << "tw";
    postData << "ctl00_MainContent_WC_Student_JsChartViewerState" << "3*3492*3791*00*0";
    postData << "ctl00_MainContent_WC_Student_callBackURL" << "/tw/LearnAssist_Grades.aspx?cdLoopBack=1";
    postData << "ctl00_MainContent_WC_Median_JsChartViewerState" << "3*3492*3791*00*0";
    postData << "ctl00_MainContent_WC_Median_callBackURL" << "/tw/LearnAssist_Grades.aspx?cdLoopBack=1";
    postData << "__ASYNCPOST" << "true";
    postData << QString::asprintf("ctl00$MainContent$DL_Smester$ctl0%d$btnSmester", n) << (map.value(s1) + QUrl::fromPercentEncoding("%20%E5%AD%B8%E5%B9%B4%E5%BA%A6%E7%AC%AC%20") + map.value(s2) + QUrl::fromPercentEncoding("%20%E5%AD%B8%E6%9C%9F"));

    lastYear = map.value(s1);
    lastSemester = map.value(s2);

    return postData;
}

QStringList LearningProtfolio::findLearningProtfolioValue(const QString &source, QMap<QString, QString> *in_out_map)
{
    QStringList retValue;

    QRegularExpression mainRE("<input type=\"\\S*?\" name=\"(\\S+?)\" id=\"(\\S+?)\" value=\"(.*?)\" />");
    QRegularExpressionMatchIterator mainIt = mainRE.globalMatch(source);

    while(mainIt.hasNext()){
        QRegularExpressionMatch mainMatch = mainIt.next();
        QString name = mainMatch.captured(1);
        QString id = mainMatch.captured(2);
        QString value = mainMatch.captured(3);

        if(in_out_map != 0){
            for(int i=0;i<in_out_map->size();i++){
                if(in_out_map->keys().at(i) == id){
                    //qDebug() << id << value;
                    in_out_map->remove(id);
                    in_out_map->insert(id, value);
                }
            }
        }
        //qDebug() << id << value;

        retValue << name << value;
    }

    return retValue;
}
