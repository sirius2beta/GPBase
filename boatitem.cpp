#include "boatitem.h"

BoatItem::BoatItem(QObject *parent)
    : QObject{parent},
      primaryConnected(false)
{
}

BoatItem::~BoatItem()
{
    qDebug()<<"BoatItem::deleted";
}

void BoatItem::setName(QString name)
{
    _name = name;
    emit nameChanged(_ID, name);
}

void BoatItem::setID(int ID)
{
    _ID = ID;
    emit IDChanged(ID);
}

void BoatItem::setPIP(QString PIP)
{
    _PIP = PIP;
    emit IPChanged(true);
}

void BoatItem::setSIP(QString SIP)
{
    _SIP = SIP;
    emit IPChanged(false);
}

void BoatItem::setCurrentIP(QString currentIP)
{
    _currentIP = currentIP;
    emit CurrentIPChanged(currentIP);
}

void BoatItem::setOS(int OS)
{
    _OS = OS;
}

Peripheral BoatItem::getPeriperalbyID(int ID)
{

    for(int i = 0; i<peripherals.size(); i++){
        if(peripherals[i].ID == ID){
            return peripherals[i];
        }
    }

    return Peripheral();
}

void BoatItem::connect(bool isPrimary)
{
    if(isPrimary){
        _currentIP = _PIP;
        primaryConnected = true;

    }
    qDebug()<<"BoatItem::connect "<<(isPrimary?"Primary":"Secondary")<<" "<<QString::number(_ID);
    emit connected(_ID, isPrimary);
    if(isPrimary){
        emit connectionChanged();
    }else{
        if(_currentIP != _PIP){
            emit connectionChanged();
        }
    }
}

void BoatItem::disconnect(bool isPrimary)
{
    if(isPrimary){
        primaryConnected = false;
    }else{
    }
    qDebug()<<"BoatItem::disconnect "<<(isPrimary?"Primary":"Secondary")<<" "<<QString::number(_ID);
    emit disconnected(_ID, isPrimary);

    if(isPrimary){
        emit connectionChanged();
    }

}

Device& BoatItem::getDevbyID(int ID)
{

    for(int i = 0; i<devices.size(); i++){
        if(devices[i].ID == ID){
            return devices[i];
        }
    }
}
