#ifndef GPBCORE_H
#define GPBCORE_H

#include <QObject>
#include "boatmanager.h"
#include "boatsetting.h"
#include "networkmanager.h"
#include "sensorwidget.h"
#include "heartbeat.h"
#include "sensorwidget.h"
#include "dnvideomanager.h"

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
    DNVideoManager* videoManager() { return _videoManager; }
    QString config() {return _config;}

signals:
    void connectionChanged();
public slots:
    void onConnectionTypeChanged();
    void onConnected(int ID, bool isprimary);
    void onDisonnected(int ID, bool isprimary);

private:
    QSettings* settings;

    QString _config;

    BoatManager* _boatManager;
    BoatSetting* _boatSetting;
    NetworkManager* _networkManager;
    DNVideoManager* _videoManager;

};

#endif // GPBCORE_H
