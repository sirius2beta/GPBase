#ifndef BOATS_H
#define BOATS_H

#include <QWidget>
#include <QDebug>
#include "device.h"



class _Boat{
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

class Boats
{
public:
    Boats();
    ~Boats();
    _Boat* addBoat(int ID);
    void deleteBoat(int ID);
    _Boat* getBoatbyName(QString boatname);
    _Boat* getBoatbyIndex(int index);
    _Boat* getBoatbyID(int ID);

    QString CurrentIP(QString boatname);
    int size();

private:
    QVector<_Boat*> boatList;
};

#endif // BOATS_H
