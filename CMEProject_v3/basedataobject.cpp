#include "basedataobject.h"

BaseDataObject::BaseDataObject()
{

}

QJsonObject& BaseDataObject::getJsonData()
{
    return jsonData;
}
