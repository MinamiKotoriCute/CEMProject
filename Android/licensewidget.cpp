#include "licensewidget.h"

#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QDebug>

LicenseWidget::LicenseWidget(QWidget *parent) : QWidget(parent)
{
    //QTextEdit *textEdit = new QTextEdit((QStringLiteral("本程式非學校官方開發。\n" "我同意授權程式設計者，以任何方式使用蒐集到的任何資料，以用來做更多的分析。")));
    QCheckBox *checkbox = new QCheckBox(QStringLiteral("我同意使用條款"));
    QPushButton *licenseButton = new QPushButton(QStringLiteral("使用條款"));
    connect(licenseButton, &QPushButton::clicked, this, [=]{
        QString str = QStringLiteral("本程式非學校開發，是私人設計。\n我同意授權程式設計者，以任何方式使用蒐集到的任何資料，用來做更多的分析。");
        QMessageBox::about(this, QStringLiteral("使用條款"), str);
    });
    QPushButton *button = new QPushButton(QStringLiteral("開始使用"));
    button->setEnabled(false);
    connect(button, &QPushButton::clicked, this, [=]{
        if(checkbox->isChecked()){
            emit ok();
        }
    });

    connect(checkbox, &QCheckBox::toggled, button, &QPushButton::setEnabled);

    QHBoxLayout *hb = new QHBoxLayout;
    hb->addWidget(checkbox);
    hb->addWidget(licenseButton);
    hb->addWidget(button);

    QVBoxLayout *vb = new QVBoxLayout;
    vb->addWidget(new QLabel(QStringLiteral("蒐集個人修課成績\n期待以統計的方法做出新版選課密集\n（尚未完成）")));
    vb->addLayout(hb);
    setLayout(vb);
}
