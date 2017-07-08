#ifndef LICENSEWIDGET_H
#define LICENSEWIDGET_H

#include <QWidget>

class LicenseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LicenseWidget(QWidget *parent = 0);

signals:
    void ok();

public slots:
};

#endif // LICENSEWIDGET_H
