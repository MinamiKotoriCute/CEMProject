#include "popularitywidget.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>

PopularityWidget::PopularityWidget(QWidget *parent) : QWidget(parent)
{
    QLabel *electiveLabel = new QLabel(QStringLiteral("選課代碼"));
    edit = new QLineEdit;
    button = new QPushButton(QStringLiteral("查詢"));

    connect(button, &QPushButton::clicked, this, [=]{
        int n = edit->text().toInt();
        if(n>0 && n<10000){
            button->setEnabled(false);
            emit send(edit->text());
            edit->clear();
            return;
        }

        QMessageBox::about(0, QStringLiteral("輸入錯誤"), QStringLiteral("請輸入正確的選課代碼"));
    });
    showlabel = new QLabel;

    QHBoxLayout *hb = new QHBoxLayout;
    hb->addWidget(electiveLabel);
    hb->addWidget(edit);
    hb->addWidget(button);

    QVBoxLayout *vb = new QVBoxLayout;
    vb->addLayout(hb);
    vb->addWidget(new QLabel(QStringLiteral("友好度：數字越大越好。")));
    vb->addWidget(showlabel);

    setLayout(vb);
}

void PopularityWidget::get(QStringList strList)
{
    qDebug() << "PopularityWidget::get";
    button->setEnabled(true);
    if(strList.size() == 3){
        QString s = QStringLiteral("課程代碼：") + edit->text() + "\n" +
                    QStringLiteral("課程名稱：") + strList.at(0) + "\n" +
                    QStringLiteral("老師：") + strList.at(1) + "\n" +
                    QStringLiteral("友好度：");

        if(static_cast<QString>(strList.at(2)).isEmpty()){
            s+=QStringLiteral("沒有歷史資料");
        }
        else{
            double friendless = static_cast<QString>(strList.at(2)).toDouble();
            s+=QString::number(friendless * 100) + "%";
        }

        showlabel->setText(s);
    }
    else{
        showlabel->setText(QStringLiteral("找不到課程"));
    }
}
