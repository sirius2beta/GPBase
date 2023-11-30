#include "device.h"

Device::Device(): ID(-1),type(0)
{
    pinUIDList = {};
}

Device::Device(const Device &d)
{
    this->deviceName = d.deviceName;
    this->ID = d.ID;
    this->settings = d.settings;
    this->type = d.type;
    this->pinUIDList = d.pinUIDList;
}

int Device::addPin(int boardID, int pinID)
{
    //return UID
    pinUIDList.append(boardID*256+pinID);
    return boardID*256+pinID;
}

int Device::addPin(int UID)
{
    //return UID
    pinUIDList.append(UID);
    return UID;
}


bool Device::operator== (const Device& lDev)
{
    if(this->ID == lDev.ID){
        return true;
    }
    return false;
}

Device::~Device()
{
    qDebug()<<"device deleted";
}

int Device::boardofPin(int index)
{
    if(index<pinUIDList.size()){
        return pinUIDList[index]/256;
    }else{
        return -1;
    }
}

int Device::pinIDofPin(int index)
{
    if(index<pinUIDList.size()){
        return pinUIDList[index]%256;
    }else{
        return -1;
    }
}

QString Device::typeName()
{
    if(this->type == DEV::SENSOR){
        return "sensor";
    }else if(this->type == DEV::SERVO){
        return "servo";
    }else if(this->type == DEV::STEPPER){
        return "stepper";
    }

    return "unknown";

}

Peripheral::Peripheral(): ID(0), boardName(QString()), boardType(QString())
{
}

Peripheral::Peripheral(const Peripheral &p)
{
    this->ID = p.ID;
    this->boardName = p.boardName;
    this->boardType = p.boardType;
    this->pinSelected = p.pinSelected;
}


