#include "overviewwidget.h"

#include <QTreeWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QDateTime>

#include "Control/control.h"
#include "Control/controlthread.h"

OverviewWidget::OverviewWidget(QList<ControlThread *> &controlThreads, QWidget *parent)
    : QWidget(parent),
      controlThreads(controlThreads)
{
    setMinimumSize(400, 300);

    QPushButton *updateButton = new QPushButton("Update");
    connect(updateButton, &QPushButton::clicked, this, [=]{
        createTreeWidget();
    });
    QPushButton *expandAllButton = new QPushButton("Expand All");
    connect(expandAllButton, &QPushButton::clicked, this, [=]{
        treeWidget->expandAll();
    });

    treeWidget = new QTreeWidget;

    QHBoxLayout *hb = new QHBoxLayout;
    hb->addWidget(expandAllButton);
    hb->addWidget(updateButton);

    QVBoxLayout *vb = new QVBoxLayout;
    vb->addLayout(hb);
    vb->addWidget(treeWidget);

    setLayout(vb);
    createTreeWidget();
}

void OverviewWidget::createTreeWidget()
{
    treeWidget->clear();

    // 設定欄位名稱
    QStringList columnTitle;
    columnTitle.append("Name");
    columnTitle.append("Detail");
    treeWidget->setHeaderLabels(columnTitle);

    foreach (ControlThread *controlThread, controlThreads) {
        QStringList topLevelItemStrList;
        topLevelItemStrList.append(QString::asprintf("%p", controlThread->getThreadId()));
        topLevelItemStrList.append(controlThread->getCreateDateTime().toString());
        QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(topLevelItemStrList);
        {
            Control *control = controlThread->getContorl();
            topLevelItem->addChild(new QTreeWidgetItem({"home number", control->getCourseSearchServerNumber()}));
            topLevelItem->addChild(new QTreeWidgetItem({"user name", control->getUserName()}));
        }
        treeWidget->addTopLevelItem(topLevelItem);
    }
}
