﻿#ifndef GPBCORE_H
#define GPBCORE_H

#include <QObject>
#include "boatmanager.h"
#include "boatsetting.h"
#include "networkmanager.h"
#include "sensorwidget.h"
#include "heartbeat.h"
#include "sensorwidget.h"

class GPBCore : public QObject
{
    Q_OBJECT
public:
    explicit GPBCore(QObject *parent = nullptr, QString config = 0);
    ~GPBCore();

    void init();

    BoatManager* boatManager() { return _boatManager; }
    BoatSetting* boatSetting() { return _boatSetting; }
    NetworkManager* networkManager() { return _networkManager; }
    SensorWidget* sensorWidget() { return _sensorwidget; }
    QString config() {return _config;}

signals:
    void connectionChanged();
public slots:
    void onNewBoat(BoatItem* newboat);
    void onConnectionTypeChanged();
    void onConnected(int ID, bool isprimary);
    void onDisonnected(int ID, bool isprimary);

private:
    QSettings* settings;

    QString _config;

    BoatManager* _boatManager;
    BoatSetting* _boatSetting;
    NetworkManager* _networkManager;
    SensorWidget* _sensorwidget;

    SensorWidget* _sensorWidget;

};

#endif // GPBCORE_H
