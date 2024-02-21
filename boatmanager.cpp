#include "boatmanager.h"
#include "gpbcore.h"

BoatManager::BoatManager(QObject* parent, GPBCore *core): QObject(parent)
{
    _core = core;

}

BoatManager::~BoatManager()
{


}

BoatItem* BoatManager::addBoat(int ID, QString boatname, QString PIP, QString SIP)
{
    BoatItem* boat = new BoatItem(this);
    boat->setID(ID);
    boat->setName(boatname);
    boat->setPIP(PIP);
    boat->setSIP(SIP);
    boatList.append(boat);
    emit boatAdded(boat);
    return boat;
}

void BoatManager::deleteBoat(int ID)
{
    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->ID() == ID){
            boatList.remove(i);
        }
    }
}

BoatItem* BoatManager::getBoatbyIndex(int index)
{
    if(index > boatList.size()){
        return 0;
    }
    return boatList[index];
}

BoatItem* BoatManager::getBoatbyID(int ID)
{

    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->ID() == ID){
            return boatList[i];
        }
    }

    return 0;
}

int BoatManager::getIDbyInex(int index)
{
    return boatList[index]->ID();
}

int BoatManager::getIndexbyID(int ID)
{
    for(int i = 0; i < boatList.size(); i++){
        if(boatList[i]->ID() == ID){
            return i;
        }
    }
    return -1;
}



QString BoatManager::CurrentIP(QString boatname)
{
    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->name() == boatname){
            return boatList[i]->currentIP();
        }
    }
    return QString();
}

int BoatManager::size()
{
    return boatList.size();
}


