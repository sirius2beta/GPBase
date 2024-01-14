#ifndef BOAT_H
#define BOAT_H

#include <QWidget>
#include "device.h"

class Boat{
public:
    QString boatName;
    int ID;
    QString PIP;
    QString SIP;
    QString CurrentIP;
    int OS;

    Peripheral getPeriperalbyID(int ID);
    QList<Peripheral> peripherals;
    QList<Device> devices;
    Device& getDevbyID(int ID);
};

#endif // BOAT_H
