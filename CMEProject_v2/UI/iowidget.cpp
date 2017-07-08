#include "iowidget.h"

#include <QTextEdit>
#include <QVBoxLayout>
#include <QDebug>

IOWidget::IOWidget(QWidget *parent) : QWidget(parent)
{
    textEdit = new QTextEdit;

    QVBoxLayout *vb = new QVBoxLayout;
    vb->addWidget(textEdit);
    setLayout(vb);
}

void IOWidget::print(QString str)
{
    qDebug() << str;
    textEdit->append(str);
}
