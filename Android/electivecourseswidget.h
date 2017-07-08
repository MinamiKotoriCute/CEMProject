#ifndef ELECTIVECOURSESWIDGET_H
#define ELECTIVECOURSESWIDGET_H

#include <QWidget>

class ElectiveCoursesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ElectiveCoursesWidget(QList<QStringList> data, QWidget *parent = 0);

signals:
    void logout();

public slots:
};

#endif // ELECTIVECOURSESWIDGET_H
