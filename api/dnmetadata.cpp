#include "dnmetadata.h"



DNMetaData::DNMetaData(QObject *parent)
    : QObject{parent}
{

}

DNMetaData::ValueType_t DNMetaData::intToType(int i)
{
    DNMetaData::ValueType_t dataType = DNMetaData::valueTypeUint32;
    if(i == 0){
        dataType = DNMetaData::valueTypeUint32;
    }else if(i == 1){
        dataType = DNMetaData::valueTypeFloat;
    }else if(i == 2){
        dataType = DNMetaData::valueTypeBool;
    }
    return dataType;
}

DNMetaData::ValueType_t DNMetaData::stringToType(QString s)
{
    DNMetaData::ValueType_t dataType = DNMetaData::valueTypeUint32;
    if(s == "int"){
        dataType = DNMetaData::valueTypeUint32;
    }else if(s == "float"){
        dataType = DNMetaData::valueTypeFloat;
    }else if(s == "bool"){
        dataType = DNMetaData::valueTypeBool;
    }
    return dataType;
}
