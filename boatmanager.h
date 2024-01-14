#ifndef BOATMANAGER_H
#define BOATMANAGER_H

#include <QWidget>
#include <QDebug>
#include "boat.h"


class BoatManager
{
public:
    BoatManager();
    ~BoatManager();
    Boat* addBoat(int ID);
    void deleteBoat(int ID);

    Boat* getBoatbyIndex(int index);
    Boat* getBoatbyID(int ID);

    QString CurrentIP(QString boatname);
    int size();
signals:


private:
    QVector<Boat*> boatList;
};

#endif // BOATMANAGER_H
