#ifndef BOATMANAGER_H
#define BOATMANAGER_H

#include <QObject>
#include <QDebug>
#include <QSettings>
#include <QStandardItemModel>
#include "boatitem.h"

class GPBCore;


class BoatManager: public QObject
{
    Q_OBJECT
public:
    BoatManager(QObject* parent = nullptr, GPBCore* core = nullptr);
    ~BoatManager();

    QAbstractItemModel* model() const {return boatItemModel;}
    void init();
    BoatItem* addBoat(int ID, QString boatname, QString PIP, QString SIP);
    void deleteBoat(int index);

    BoatItem* getBoatbyIndex(int index);
    BoatItem* getBoatbyID(int ID);
    int getIDbyInex(int index);
    int getIndexbyID(int ID);

    QString CurrentIP(QString boatname);
    int size();
public slots:
    void onConnected(int ID, bool isprimary);
    void onDisonnected(int ID, bool isprimary);
signals:
    void boatAdded(BoatItem* newboat);
    void boatDeleted(int index);
private:
    QSettings *settings;
    QStandardItemModel* boatItemModel;
    QVector<BoatItem*> boatList;
    GPBCore* _core;
};

#endif // BOATMANAGER_H
