#ifndef GPBCORE_H
#define GPBCORE_H

#include <QObject>
#include "boatmanager.h"
#include "boatsetting.h"
#include "networkmanager.h"
#include "sensorwidget.h"
#include "heartbeat.h"
#include "videowindow.h"

class GPBCore : public QObject
{
    Q_OBJECT
public:
    explicit GPBCore(QObject *parent = nullptr, QString config = 0);
    ~GPBCore();
    BoatManager* boatList() { return _boatList; }
    BoatSetting* boatSetting() { return _boatSetting; }
    NetworkManager* networkManager() { return _networkManager; }
    SensorWidget* sensorWidget() { return _sensorwidget; }

signals:
    void connectionChanged();
protected slots:
    void onNewBoat(Boat *newboat);
    void onConnectionTypeChanged();
    void onConnected(int ID, bool isprimary);
    void onDisonnected(int ID, bool isprimary);

private:
    QSettings* settings;

    QString _config;

    BoatManager* _boatList;
    BoatSetting* _boatSetting;
    NetworkManager* _networkManager;
    SensorWidget* _sensorwidget;
    HeartBeat* _primaryHeartBeat;
    HeartBeat* _secondaryHeartBeat;

};

#endif // GPBCORE_H
