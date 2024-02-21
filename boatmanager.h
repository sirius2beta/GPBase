#ifndef BOATMANAGER_H
#define BOATMANAGER_H

#include <QObject>
#include <QDebug>
#include "boatitem.h"

class GPBCore;


class BoatManager: public QObject
{
    Q_OBJECT
public:
    BoatManager(QObject* parent = nullptr, GPBCore* core = nullptr);
    ~BoatManager();
    BoatItem* addBoat(int ID);
    void deleteBoat(int ID);

    BoatItem* getBoatbyIndex(int index);
    BoatItem* getBoatbyID(int ID);
    int getIDbyInex(int index);
    int getIndexbyID(int ID);

    QString CurrentIP(QString boatname);
    int size();
signals:


private:
    QVector<BoatItem*> boatList;
    GPBCore* _core;
};

#endif // BOATMANAGER_H
