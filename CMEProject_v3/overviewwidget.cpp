#include "overviewwidget.h"

#include <QTreeWidget>
#include <QJsonObject>
#include <QDebug>
#include <QVBoxLayout>

OverviewWidget::OverviewWidget(QWidget *parent) : QWidget(parent)
{
    treeWidget = new QTreeWidget;
    QVBoxLayout *vb = new QVBoxLayout;
    vb->addWidget(treeWidget);
    setLayout(vb);
}

void OverviewWidget::add(QJsonObject *data, QString name)
{
    datas.push_back(data);
    dataNames.push_back(name);
}

void OverviewWidget::createTreeWidget()
{
    treeWidget->clear();
    qDebug() << 2;

    // 設定欄位名稱
    QStringList columnTitle;
    columnTitle.append("Key");
    columnTitle.append("Value");
    treeWidget->setHeaderLabels(columnTitle);
    qDebug() << 3;

    //QStringList topLevelItemStrList;
    //topLevelItemStrList.append(QString::asprintf("%p", controlThread->getThreadId()));
    //topLevelItemStrList.append(controlThread->getCreateDateTime().toString());
    //QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(topLevelItemStrList);

    for(int i=0;i<datas.count();i++){
        qDebug() << 14 << i;
        qDebug() << *(datas.at(i));
        qDebug() << dataNames.at(i);
        treeWidget->addTopLevelItem(createTreeWidget(*(datas.at(i)), dataNames.at(i)));
    }

    treeWidget->expandAll();
}

QTreeWidgetItem* OverviewWidget::createTreeWidget(QJsonObject data, QString title){
    qDebug() << 1;
    QStringList parentItemStrList;
    parentItemStrList.append(title);
    parentItemStrList.append("");
    QTreeWidgetItem *parentItem = new QTreeWidgetItem(parentItemStrList);
    for(int i=0;i<data.count();i++){
        QTreeWidgetItem *childIteml;
        QString key = data.keys().at(i);
        QJsonValue value = data.value(key);
        if(value.isObject())
            childIteml = createTreeWidget(value.toObject(), key);
        else{
            QStringList childItemStrList;
            childItemStrList.append(key);
            childItemStrList.append(value.toString());
            childIteml = new QTreeWidgetItem(childItemStrList);
        }
        parentItem->addChild(childIteml);
    }
    return parentItem;
}
