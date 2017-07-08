#ifndef IOWIDGET_H
#define IOWIDGET_H

#include <QWidget>

class QTextEdit;

class IOWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IOWidget(QWidget *parent = 0);

signals:

public slots:
    void print(QString str);

private:
    QTextEdit *textEdit;
};

#endif // IOWIDGET_H
