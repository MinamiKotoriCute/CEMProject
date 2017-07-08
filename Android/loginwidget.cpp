#include "loginwidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    QLabel *usernameLabel = new QLabel(QStringLiteral("NID帳號:"));
    QLabel *passwordLabel = new QLabel(QStringLiteral("NID密碼:"));
    QLineEdit *usernameEdit = new QLineEdit;
    QLineEdit *passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    loginButton = new QPushButton(QStringLiteral("登入"));

    //QCheckBox *usernameCheckBox = new QCheckBox(QStringLiteral("記住我的帳號"));
    //QCheckBox *passwordCheckBox = new QCheckBox(QStringLiteral("記住我的帳號"));

    QHBoxLayout *hb1 = new QHBoxLayout;
    hb1->addWidget(usernameLabel);
    hb1->addWidget(usernameEdit);
    QHBoxLayout *hb2 = new QHBoxLayout;
    hb2->addWidget(passwordLabel);
    hb2->addWidget(passwordEdit);

    QVBoxLayout *vb = new QVBoxLayout;
    vb->addLayout(hb1);
    vb->addLayout(hb2);
    //vb->addLayout(usernameCheckBox);
    //vb->addLayout(passwordCheckBox);
    vb->addWidget(loginButton);

    setLayout(vb);

    connect(loginButton, &QPushButton::clicked, this, [=]{
        if(usernameEdit->text().isEmpty()){
            QMessageBox::about(this, "lose soming...", "enter username");
            return;
        }
        if(passwordEdit->text().isEmpty()){
            QMessageBox::about(this, "lose soming...", "enter password");
            return;
        }

        loginButton->setEnabled(false);
        emit login(usernameEdit->text(), passwordEdit->text());
    });
}

LoginWidget::~LoginWidget()
{
    qDebug() << "~LoginWidget()";
}

void LoginWidget::reset()
{
    loginButton->setEnabled(true);
}
