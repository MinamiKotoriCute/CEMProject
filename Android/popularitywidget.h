#ifndef POPULARITYWIDGET_H
#define POPULARITYWIDGET_H

#include <QWidget>

class QLabel;
class QPushButton;
class QLineEdit;

class PopularityWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PopularityWidget(QWidget *parent = 0);

signals:
    void send(QString);

public slots:
    void get(QStringList strList);

private:
    QLineEdit *edit;
    QLabel *showlabel;
    QPushButton *button;
};

#endif // POPULARITYWIDGET_H
