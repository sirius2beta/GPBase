#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <QObject>
#include <QSettings>
#include <QStandardItemModel>
#include "sensoritem.h"


class GPBCore;

class SensorManager : public QObject
{
    Q_OBJECT
public:
    explicit SensorManager(QObject *parent = nullptr, GPBCore *core = nullptr);
    void setSensorData(int ID, int datatype, int sensortype, QByteArray data);
    void init();
    int count() { return _sensorList.size(); }
    SensorItem* getSensor(int index){ return _sensorList[index];}
    void addSensor(int boatID, int sensorType, QString sensorName);
    void deleteSensor(int index);
    QAbstractItemModel* sensorTypeModel() { return _sensorTypeModel;}
    enum dataType{ int_type = 0, float_type = 1};


signals:
    void sensorDataChanged(int ID, int datatype, int sensortype, QByteArray data);

public slots:
    void onSensorMsg(int ID, QByteArray data);

private:
    QSettings* settings;
    QVector<SensorItem*> _sensorList;
    GPBCore* _core;
    QStandardItemModel* _sensorTypeModel;
};

#endif // SENSORMANAGER_H
