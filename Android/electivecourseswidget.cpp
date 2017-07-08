#include "electivecourseswidget.h"

#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

ElectiveCoursesWidget::ElectiveCoursesWidget(QList<QStringList> data, QWidget *parent) : QWidget(parent)
{
    QTableWidget *tableWidget = new QTableWidget;
    tableWidget->setRowCount(data.size());
    tableWidget->setColumnCount(4);

    QStringList header;
    header.append("elective code");
    header.append("course");
    header.append("teacher");
    header.append("friendliness");
    tableWidget->setHorizontalHeaderLabels(header);

    for(int i=0;i<data.size();i++){
        QStringList classInformation = data.at(i);
        for(int j=0;j<qMin(classInformation.size(), 4);j++){
            //qDebug() << i << j << static_cast<QString>(classInformation.at(j));
            tableWidget->setItem(i, j, new QTableWidgetItem(static_cast<QString>(classInformation.at(j))));
        }
    }

    QPushButton *logoutButton = new QPushButton("logout");
    connect(logoutButton, &QPushButton::clicked, this, &ElectiveCoursesWidget::logout);

    QVBoxLayout *vb = new QVBoxLayout;
    vb->addWidget(tableWidget);
    vb->addWidget(logoutButton);
    setLayout(vb);
}
