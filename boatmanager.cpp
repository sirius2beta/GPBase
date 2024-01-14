#include "boatmanager.h"


BoatManager::BoatManager()
{

}

BoatManager::~BoatManager()
{
    if(!boatList.empty()){
        for(int i = 0; i < boatList.size(); i++){
            delete boatList[i];
        }
    }

}

Boat* BoatManager::addBoat(int ID)
{
    Boat* boat = new Boat;
    boat->ID = ID;
    boatList.append(boat);
    return boat;
}

void BoatManager::deleteBoat(int ID)
{
    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->ID == ID){
            boatList.remove(i);
        }
    }
}

Boat* BoatManager::getBoatbyIndex(int index)
{
    if(index > boatList.size()){
        return 0;
    }
    return boatList[index];
}

Boat* BoatManager::getBoatbyID(int ID)
{

    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->ID == ID){
            return boatList[i];
        }
    }

    return 0;
}

Device& Boat::getDevbyID(int ID)
{

    for(int i = 0; i<devices.size(); i++){
        if(devices[i].ID == ID){
            return devices[i];
        }
    }
}

QString BoatManager::CurrentIP(QString boatname)
{
    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->boatName == boatname){
            return boatList[i]->CurrentIP;
        }
    }
    return QString();
}

int BoatManager::size()
{
    return boatList.size();
}

Peripheral Boat::getPeriperalbyID(int ID)
{

    for(int i = 0; i<peripherals.size(); i++){
        if(peripherals[i].ID == ID){
            return peripherals[i];
        }
    }

    return Peripheral();
}
