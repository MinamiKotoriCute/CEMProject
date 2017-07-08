#ifndef OVERVIEWWIDGET_H
#define OVERVIEWWIDGET_H

#include <QWidget>

class QTreeWidget;
class QTreeWidgetItem;

class OverviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OverviewWidget(QWidget *parent = 0);

    void add(QJsonObject *data, QString name);
    void createTreeWidget();

signals:

public slots:

private:
    QTreeWidgetItem* createTreeWidget(QJsonObject data, QString title);

    QTreeWidget *treeWidget;
    QList<const QJsonObject *> datas;
    QStringList dataNames;
};

#endif // OVERVIEWWIDGET_H
