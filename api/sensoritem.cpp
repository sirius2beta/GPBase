#include "sensoritem.h"

SensorItem::SensorItem(QObject *parent)
    : QObject{parent},
    _value(DNValue())
{

}

void SensorItem::setBoatID(int ID)
{
    _boatID = ID;
    emit BoatIDSet(ID);
}

void SensorItem::setName(QString name)
{
    _value.setName(name);
    emit nameSet(name);
}

void SensorItem::setValue(DNValue value)
{
    _value = value;
    if(value.dataType() == DNMetaData::valueTypeUint32){
        emit textSet(QString::number(value.data().toUInt()));
    }else if(value.dataType() == DNMetaData::valueTypeFloat){
        emit textSet(QString::number(value.data().toFloat()));
    }else if(value.dataType() == DNMetaData::valueTypeBool){
        emit textSet(value.data().toBool()?"On":"Off");
    }
}
