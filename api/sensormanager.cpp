#include "sensormanager.h"
#include "gpbcore.h"
#include <QByteArray>
#include <QVariant>
SensorManager::SensorManager(QObject *parent, GPBCore* core)
    : QObject{parent},
    _core(core)
{
    settings = new QSettings("Ezosirius", "GPlayer_v1", this);
    _sensorTypeModel = new QStandardItemModel;

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
        SensorItem* item = new SensorItem(this);
        item->setBoatID(boatID);
        item->setName(sensorName);
        addSensor(item);

    }
    settings->endArray();
    settings->endGroup();
    qDebug()<<"sensorManager::init(): Initiate complete";
}

void SensorManager::addSensor(SensorItem* sensorItem)
{
    qDebug()<<"gettttt  "<<sensorItem->value().dataType();
    _sensorList.append(sensorItem);
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
            memcpy(&sensortype, data.left(4).data(), sizeof(int));
            qDebug()<<"get sss "<<_sensorList.size();
        }else if(readorder == 1){
            QByteArray sensordata(data.data()+i,1);
            readorder = -1;
            DNValue value = _core->configManager()->sensorTypeList()[sensortype];
            //value.setValue(QVariant(data.right(4).data()));


            for(int i = 0; i< _sensorList.size(); i++){
                qDebug()<<"get  "<<_sensorList[i]->value().dataType();
                if(_sensorList[i]->boatID() == ID && _sensorList[i]->value().SensorType() == sensortype){
                    if(_sensorList[i]->value().dataType() == DNMetaData::valueTypeUint32){
                        int i;
                        memcpy(&i, data.right(4).data(), sizeof(int));
                        value.setValue(QVariant(i));
                        qDebug()<<"get iii "<<i;
                    }else if(_sensorList[i]->value().dataType() == DNMetaData::valueTypeFloat){
                        float f;
                        memcpy(&f, data.right(4).data(), sizeof(float));
                        value.setValue(QVariant(f));
                        qDebug()<<"get fff "<<i;
                    }else if(_sensorList[i]->value().dataType() == DNMetaData::valueTypeBool){
                        int i;
                        memcpy(&i, data.right(4).data(), sizeof(int));
                        value.setValue(QVariant(i));
                        qDebug()<<"get fff "<<i;
                    }
                    _sensorList[i]->setValue(value);

                }
            }
        }
        readorder ++;
    }
}
