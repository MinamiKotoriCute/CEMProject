#ifndef BASEDATAOBJECT_H
#define BASEDATAOBJECT_H

#include <QJsonObject>

class BaseDataObject
{
public:
    BaseDataObject();

    QJsonObject& getJsonData();

protected:
    QJsonObject jsonData;
};

#endif // BASEDATAOBJECT_H
