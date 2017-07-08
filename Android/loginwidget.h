#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

class QPushButton;

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

signals:
    void login(QString, QString);

public slots:
    void reset();

private:
    QPushButton *loginButton;
};

#endif // LOGINWIDGET_H
