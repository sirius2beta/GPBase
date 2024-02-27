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
    Q_PROPERTY(QAbstractItemModel* model READ model)
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
    //void setConnectionType(int connectiontype);
    int size();
signals:
    void BoatAdded();
    void connectionTypeChanged(int connectiontype);
public slots:
    void onBoatNameChange(int ID, QString newname);
    void onIPChanged(int ID, bool primary);
    void onConnected(int ID, bool isprimary);
    void onDisonnected(int ID, bool isprimary);
    void onConnectionTypeChanged(int connectiontype);

private:
    QSettings *settings;
    QStandardItemModel* boatItemModel;
    QVector<BoatItem*> boatList;
    int _connectionType;
    GPBCore* _core;
};

#endif // BOATMANAGER_H
