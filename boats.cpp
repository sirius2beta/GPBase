#include "boats.h"


Boats::Boats()
{

}

Boats::~Boats()
{
    if(!boatList.empty()){
        for(int i = 0; i < boatList.size(); i++){
            delete boatList[i];
        }
    }

}

_Boat* Boats::addBoat(int ID)
{
    _Boat* boat = new _Boat;
    boat->ID = ID;
    boatList.append(boat);
    return boat;
}

void Boats::deleteBoat(int ID)
{
    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->ID == ID){
            boatList.remove(i);
        }
    }
}
_Boat* Boats::getBoatbyName(QString boatname)
{
    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->boatName == boatname){

            return boatList[i];
        }
    }
    return 0;
}
_Boat* Boats::getBoatbyIndex(int index)
{
    if(index > boatList.size()){
        return 0;
    }
    return boatList[index];
}

_Boat* Boats::getBoatbyID(int ID)
{

    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->ID == ID){
            return boatList[i];
        }
    }

    return 0;
}

Device& _Boat::getDevbyID(int ID)
{

    for(int i = 0; i<devices.size(); i++){
        if(devices[i].ID == ID){
            return devices[i];
        }
    }
}

QString Boats::CurrentIP(QString boatname)
{
    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->boatName == boatname){
            return boatList[i]->CurrentIP;
        }
    }
    return QString();
}

int Boats::size()
{
    return boatList.size();
}

Peripheral _Boat::getPeriperalbyID(int ID)
{

    for(int i = 0; i<peripherals.size(); i++){
        if(peripherals[i].ID == ID){
            return peripherals[i];
        }
    }

    return Peripheral();
}
