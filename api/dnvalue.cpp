#include "dnvalue.h"

DNValue::DNValue(QObject *parent)
    : QObject{parent},
    _sensorType(-1),
    _name(QString()),
    _rawValue(0),
    _type(DNMetaData::valueTypeUint32)
{

}

DNValue::DNValue(int sensorType, QString name, DNMetaData::ValueType_t type, QObject *parent)
    : QObject{parent},
    _sensorType(sensorType),
    _name(name),
    _rawValue(0),
    _type(type)
{

}

DNValue::DNValue(const DNValue& other, QObject *parent)
{
    *this = other;
}

const DNValue& DNValue::operator =(const DNValue& other)
{
    _sensorType = other._sensorType;
    _name = other._name;
    _rawValue = other._rawValue;
    _type = other._type;
    return *this;
}

DNValue::~DNValue()
{

}
