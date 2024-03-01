#include "sensormanager.h"
#include "gpbcore.h"
#include <QByteArray>

SensorManager::SensorManager(QObject *parent, GPBCore* core)
    : QObject{parent},
    _core(core)
{
    settings = new QSettings("Ezosirius", "GPlayer_v1", this);
    _sensorTypeModel = new QStandardItemModel;

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

    for(int i = 0; i< _core->configManager()->sensorTypeList().size(); i++){
        QString name = _core->configManager()->sensorTypeList()[i].name();
        QStandardItem* item = new QStandardItem(name);
        _sensorTypeModel->setItem(i,0,item);
        qDebug()<<"  - Add sensor name: "<<name;
    }

    int size = settings->beginReadArray("sensor");

    for(int i = 0; i < size; i++){
        settings->setArrayIndex(i);
        int boatID = settings->value("boatID").toInt();
        DNMetaData::ValueType_t sensorType = DNMetaData::intToType(settings->value("sensorType").toInt());
        QString sensorName = settings->value("sensorName").toString();
        addSensor(boatID, sensorType, sensorName);

    }
    settings->endArray();
    settings->endGroup();
    qDebug()<<"sensorManager::init(): Initiate complete";
}

void SensorManager::addSensor(int boatID, DNMetaData::ValueType_t sensorType, QString sensorName)
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
    DNMetaData::ValueType_t datatype;
    int sensortype;
    QByteArray sensordata;

    int readorder = 0;
    for(int i = 0; i < data.size(); i++){
        if(readorder == 0){
            datatype = DNMetaData::intToType(int(data[i]));
        }else if(readorder == 1){
            sensortype = int(data[i]);
        }else if(readorder == 2){
            QByteArray sensordata(data.data()+i,1);
            readorder = -1;
            DNValue value = _core->configManager()->sensorTypeList()[sensortype];
            emit sensorDataChanged(ID, datatype, sensortype, sensordata);
        }
        readorder ++;
    }
}
