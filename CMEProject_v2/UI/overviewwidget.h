#ifndef OVERVIEWWIDGET_H
#define OVERVIEWWIDGET_H

#include <QWidget>

class QTreeWidget;
class ControlThread;

class OverviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OverviewWidget(QList<ControlThread *> &controlThreads, QWidget *parent = 0);

signals:

public slots:

private:
    void createTreeWidget();

    QTreeWidget *treeWidget;

    QList<ControlThread *> &controlThreads;
};

#endif // OVERVIEWWIDGET_H
