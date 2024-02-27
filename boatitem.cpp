#include "boatitem.h"

BoatItem::BoatItem(QObject *parent)
    : QObject{parent},
    primaryConnected(false),
    secondaryConnected(false),
    _connectionPriority(0),
    _linkType(0)
{
}

BoatItem::~BoatItem()
{
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
    emit IPChanged(_ID, true);
}

void BoatItem::setSIP(QString SIP)
{
    _SIP = SIP;
    emit IPChanged(_ID, false);
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

void BoatItem::setConnectionPriority(int connectionType)
{
    _connectionPriority = connectionType;
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

    bool isConnectionChanged = false;
    if(_connectionPriority == 0){
        if(primaryConnected){
            if(isPrimary){
                //keep connected
            }else{
                emit connectionChanged(_ID);
                _currentIP = _SIP;
            }
        }else{
            if(secondaryConnected){
                if(isPrimary){
                    emit connectionChanged(_ID);
                    _currentIP = _PIP;
                }else{
                    //keep connected
                }
            }else{
                if(isPrimary){
                    emit connectionChanged(_ID);
                    _currentIP = _PIP;
                }else{
                    //keep connected
                }
            }
        }
    }else{
        if(secondaryConnected){
            if(isPrimary){
                emit connectionChanged(_ID);
                _currentIP = _PIP;
            }else{
                //keep connected
            }
        }else{
            if(primaryConnected){
                if(isPrimary){
                    //keep connected
                }else{
                    emit connectionChanged(_ID);
                    _currentIP = _SIP;
                }

            }else{
                if(isPrimary){
                    //keep connected
                }else{
                    emit connectionChanged(_ID);
                    _currentIP = _SIP;
                }
            }
        }
    }

    if(isPrimary){
        primaryConnected = true;
    }else{
        secondaryConnected = true;
    }

    qDebug()<<"BoatItem::connect "<<(isPrimary?"Primary":"Secondary")<<" "<<QString::number(_ID);
    emit connected(_ID, isPrimary);

}

void BoatItem::disconnect(bool isPrimary)
{
    if(isPrimary){
        primaryConnected = false;
    }else{
        secondaryConnected = false;
    }

    if(_connectionPriority == 0){
        if(primaryConnected){
            //keep connected
        }else{
            if(secondaryConnected){
                qDebug()<<"BoatItem:: switch connection";
                emit connectionChanged(_ID);
                _currentIP = _SIP;
            }
        }
    }else{
        if(secondaryConnected){
            //keep connected
        }else{
            if(primaryConnected){
                qDebug()<<"BoatItem:: switch connection";
                emit connectionChanged(_ID);
                _currentIP = _PIP;
            }
        }
    }

    qDebug()<<"BoatItem::disconnect "<<(isPrimary?"Primary":"Secondary")<<" "<<QString::number(_ID);
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
