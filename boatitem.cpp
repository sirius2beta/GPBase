#include "boatitem.h"

BoatItem::BoatItem(QObject *parent)
    : QObject{parent}
{
}

void BoatItem::setName(QString name)
{
    _name = name;
    emit nameChanged(name);
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
    }
    emit connected(_ID, isPrimary);
}

void BoatItem::disconnect(bool isPrimary)
{
    if(isPrimary){
        _currentIP = _SIP;
    }else{
        _currentIP = _PIP;
    }
    emit disconnected(_ID, isPrimary);
}

Device& BoatItem::getDevbyID(int ID)
{

    for(int i = 0; i<devices.size(); i++){
        if(devices[i].ID == ID){
            return devices[i];
        }
    }
}
