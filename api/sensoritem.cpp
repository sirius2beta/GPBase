#include "sensoritem.h"

SensorItem::SensorItem(QObject *parent)
    : QObject{parent},
    _boatID(-1),
    _sensorType(-1),
    _name(QString())
{

}

void SensorItem::setBoatID(int ID)
{
    _boatID = ID;
    emit BoatIDSet(ID);
}

void SensorItem::setSensorType(int type)
{
    _sensorType = type;
    emit sensorTypeSet(type);
}

void SensorItem::setName(QString name)
{
    _name = name;
    emit nameSet(name);
}

void SensorItem::setData(int dataType, QByteArray data)
{
    if(dataType == 0){
        emit setText(QString::number(int(data[0])));
    }else if(dataType == 1){
        emit setText(QString::number(float(data[0])));
    }else if(dataType == 2){
        emit setText(int(data[0])?"On":"Off");
    }
}
