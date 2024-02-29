#ifndef GPBCORE_H
#define GPBCORE_H

#include <QObject>
#include "boatmanager.h"
#include "networkmanager.h"
#include "heartbeat.h"
#include "dnvideomanager.h"
#include "sensormanager.h"
#include "configmanager.h"

class GPBCore : public QObject
{
    Q_OBJECT
public:
    explicit GPBCore(QObject *parent = nullptr, QString config = 0);
    ~GPBCore();

    void init();

    BoatManager* boatManager() { return _boatManager; }
    NetworkManager* networkManager() { return _networkManager; }
    DNVideoManager* videoManager() { return _videoManager; }
    SensorManager* sensorManager() { return _sensorManager; }
    ConfigManager* configManager() { return _configManager; }
    QString config() {return _config;}


private:
    QSettings* settings;
    QString _config;
    BoatManager* _boatManager;
    NetworkManager* _networkManager;
    DNVideoManager* _videoManager;
    SensorManager* _sensorManager;
    ConfigManager* _configManager;

};

#endif // GPBCORE_H
