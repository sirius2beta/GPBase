#include "sensormanager.h"
#include "gpbcore.h"
#include <QByteArray>

SensorManager::SensorManager(QObject *parent, GPBCore* core)
    : QObject{parent},
    _core(core)
{
    settings = new QSettings("Ezosirius", "GPlayer_v1", this);

    QStandardItem* item0 = new QStandardItem("Temperature");
    QStandardItem* item1 = new QStandardItem("Humidity");
    _sensorTypeModel = new QStandardItemModel;
    //set and remove item so QCombobox can keep index updated
    _sensorTypeModel->setItem(0,0,item0);
    _sensorTypeModel->setItem(1,0,item1);
}

void SensorManager::setSensorData(int ID, int datatype, int sensortype, QByteArray data)
{
    for(int i = 0; i < _sensorList.size(); i++){
        SensorItem* sensorItem = _sensorList[i];
        if(sensorItem->boatID() == ID && sensorItem->sensorType() == sensortype){
            sensorItem->setData(datatype, data);
        }
    }
}

void SensorManager::init()
{
    qDebug()<<"sensorManager::init(): Initiating...";
    settings->beginGroup(QString("%1").arg(_core->config()));
    int size = settings->beginReadArray("sensor");

    for(int i = 0; i < size; i++){
        settings->setArrayIndex(i);
        int boatID = settings->value("boatID").toInt();
        int sensorType = settings->value("sensorType").toInt();
        QString sensorName = settings->value("sensorName").toString();
        addSensor(boatID, sensorType, sensorName);
        qDebug()<<"  - Add sensor: ID:"<<boatID<<", name:"<<sensorName;

    }
    settings->endArray();
    settings->endGroup();
    qDebug()<<"sensorManager::init(): Initiate complete";
}

void SensorManager::addSensor(int boatID, int sensorType, QString sensorName)
{
    SensorItem* sensorItem = new SensorItem(this);
    sensorItem->setBoatID(boatID);
    sensorItem->setSensorType(sensorType);
    sensorItem->setName(sensorName);
}

void SensorManager::deleteSensor(int index)
{
    _sensorList.remove(index);
}

void SensorManager::onSensorMsg(int ID, QByteArray data)
{
    int datatype;
    int sensortype;
    QByteArray sensordata;
    int readorder = 0;
    for(int i = 0; i < data.size(); i++){
        if(readorder == 0){
            datatype = int(data[i]);
        }else if(readorder == 1){
            sensortype = int(data[i]);
        }else if(readorder == 2){
            QByteArray sensordata(data.data()+i,1);
            readorder = -1;
            emit sensorDataChanged(ID, datatype, sensortype, sensordata);
        }
        readorder ++;
    }
}
