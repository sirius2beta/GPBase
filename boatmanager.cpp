#include "boatmanager.h"
#include "gpbcore.h"

BoatManager::BoatManager(QObject* parent, GPBCore *core): QObject(parent)
{
    _core = core;
    settings = new QSettings("Ezosirius", "GPlayer_v1", this);
    boatItemModel = new QStandardItemModel();
    QStringList label = {"name", "Primary", "Secondary"};
    boatItemModel->setHorizontalHeaderLabels(label);
    init();

}

BoatManager::~BoatManager()
{


}

void BoatManager::init()
{
    settings->beginGroup(QString("%1").arg(_core->config()));
    int size = settings->beginReadArray("boat");

    for(int i = 0; i < size; i++){
        qDebug()<<"boatList size:"<<boatList.size();
        settings->setArrayIndex(i);
        QString boatname = settings->value("boatname").toString();
        int ID = settings->value("ID").toInt();
        qDebug()<<"ID:"<<ID;
        QString boatPIP = settings->value("/PIP").toString();
        QString boatSIP = settings->value("/SIP").toString();

        BoatItem* boat = new BoatItem(this);
        boat->setID(ID);
        boat->setName(boatname);
        boat->setPIP(boatPIP);
        boat->setSIP(boatSIP);
        boatList.append(boat);
        int current = boatItemModel->rowCount();
        QStandardItem* item1 = new QStandardItem(boatname);
        QStandardItem* item2 = new QStandardItem(QString("SB"));
        item2->setData(boatPIP);
        item2->setBackground(QBrush(QColor(120,0,0)));
        QStandardItem* item3 = new QStandardItem(QString("SB"));
        item3->setData(boatSIP);
        item3->setBackground(QBrush(QColor(120,0,0)));
        boatItemModel->setItem(current,0,item1);
        boatItemModel->setItem(current,1,item2);
        boatItemModel->setItem(current,2,item3);

        connect(boat, &BoatItem::connected, this, &BoatManager::onConnected);
        connect(boat, &BoatItem::disconnected, this, &BoatManager::onDisonnected);

        HeartBeat* _primaryHeartBeat = new HeartBeat(boat, 50006, true, this, _core);
        _primaryHeartBeat->HeartBeatLoop();
        qDebug()<<"ck1";
        HeartBeat* _secondaryHeartBeat = new HeartBeat(boat, 50006, false, this, _core);
        _secondaryHeartBeat->HeartBeatLoop();

        connect(boat, &BoatItem::connected,  _core, &GPBCore::onConnected);
        connect(boat, &BoatItem::disconnected, _core, &GPBCore::onDisonnected);
        connect(boat, &BoatItem::IPChanged, _primaryHeartBeat, &HeartBeat::onChangeIP);
        connect(boat, &BoatItem::IPChanged, _secondaryHeartBeat, &HeartBeat::onChangeIP);



        emit boatAdded(boat);
        qDebug()<<"BoatManager:: addboat";

    }

    settings->endArray();
    settings->endGroup();
}

BoatItem* BoatManager::addBoat(int ID, QString boatname, QString PIP, QString SIP)
{
    BoatItem* boat = new BoatItem(this);


    //int count = 0;
    QVector<bool> indexfree(256, true);
    int index = 0;
    for(int i = 0; i< size(); i++){
        indexfree[getBoatbyIndex(i)->ID()] = false;

    }
    for(int i =0; i<256; i++){
        if(indexfree[i] == true){
            index = i;

            break;
        }
    }
    QString newboatname = "unknown";

    boat->setID(ID);
    boat->setName(boatname);
    boat->setPIP(PIP);
    boat->setSIP(SIP);
    boatList.append(boat);
    connect(boat, &BoatItem::connected, this, &BoatManager::onConnected);
    connect(boat, &BoatItem::disconnected, this, &BoatManager::onDisonnected);

    HeartBeat* _primaryHeartBeat = new HeartBeat(boat, 50006, true, this, _core);
    _primaryHeartBeat->HeartBeatLoop();
    HeartBeat* _secondaryHeartBeat = new HeartBeat(boat, 50006, false, this, _core);
    _secondaryHeartBeat->HeartBeatLoop();
    qDebug()<<"ck1";
    connect(boat, &BoatItem::connected,  _core, &GPBCore::onConnected);
    connect(boat, &BoatItem::disconnected, _core, &GPBCore::onDisonnected);
    connect(boat, &BoatItem::IPChanged, _primaryHeartBeat, &HeartBeat::onChangeIP);
    connect(boat, &BoatItem::IPChanged, _secondaryHeartBeat, &HeartBeat::onChangeIP);


    int current = boatItemModel->rowCount();
    QStandardItem* item1 = new QStandardItem(boatname);
    QStandardItem* item2 = new QStandardItem(QString("SB"));
    item2->setData(PIP);
    item2->setBackground(QBrush(QColor(120,0,0)));
    QStandardItem* item3 = new QStandardItem(QString("SB"));
    item3->setData(SIP);
    item3->setBackground(QBrush(QColor(120,0,0)));
    boatItemModel->setItem(current,0,item1);
    boatItemModel->setItem(current,1,item2);
    boatItemModel->setItem(current,2,item3);
    //1
    settings->beginGroup(QString("%1").arg(_core->config()));
    int size = settings->beginReadArray("boat");

    settings->endArray();
    settings->beginWriteArray("boat");
    settings->setArrayIndex(size);
    settings->setValue(QString("boatname"), newboatname);
    settings->setValue(QString("ID"), index);
    settings->setValue(QString("PIP"), "");
    settings->setValue(QString("SIP"), "");
    settings->endArray();
    settings->endGroup();

    emit boatAdded(boat);
    return boat;
}

void BoatManager::deleteBoat(int ID)
{
    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->ID() == ID){
            boatList.remove(i);
        }
    }
}

BoatItem* BoatManager::getBoatbyIndex(int index)
{
    if(index > boatList.size()){
        return 0;
    }
    return boatList[index];
}

BoatItem* BoatManager::getBoatbyID(int ID)
{

    for(int i = 0; i<boatList.size(); i++){

        if(boatList[i]->ID() == ID){

            return boatList[i];
        }
    }
    return 0;
}

int BoatManager::getIDbyInex(int index)
{
    return boatList[index]->ID();
}

int BoatManager::getIndexbyID(int ID)
{
    for(int i = 0; i < boatList.size(); i++){
        if(boatList[i]->ID() == ID){
            return i;
        }
    }
    return -1;
}



QString BoatManager::CurrentIP(QString boatname)
{
    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->name() == boatname){
            return boatList[i]->currentIP();
        }
    }
    return QString();
}

int BoatManager::size()
{
    return boatList.size();
}

void BoatManager::onConnected(int ID, bool isprimary)
{
    for(int i = 0; i < boatItemModel->rowCount();i++){
        qDebug()<<"boatItem: "<<boatItemModel->item(i,0)->text()<<"boatName: "<<getBoatbyID(ID)->name();
        if(boatItemModel->item(i,0)->text() == getBoatbyID(ID)->name()){

            if(isprimary){
                boatItemModel->item(i,1)->setText("Active");
                boatItemModel->item(i,1)->setBackground(QBrush(QColor(0,120,0)));
            }else{
                boatItemModel->item(i,2)->setText("Active");
                boatItemModel->item(i,2)->setBackground(QBrush(QColor(0,120,0)));
            }
        }
    }

}

void BoatManager::onDisonnected(int ID, bool isprimary)
{

    qDebug()<<"boatItemModel.size:"<<boatItemModel->rowCount();
    qDebug()<<"boatList.size:"<<boatList.size();
    for(int i = 0; i < boatList.size();i++){
        if(getBoatbyID(ID) == 0 || boatItemModel->item(i,0) == 0) return;
        if(boatItemModel->item(i,0)->text() == getBoatbyID(ID)->name()){

            if(isprimary){
                boatItemModel->item(i,1)->setText("SB");
                boatItemModel->item(i,1)->setBackground(QBrush(QColor(120,0,0)));
            }else{
                boatItemModel->item(i,2)->setText("SB");
                boatItemModel->item(i,2)->setBackground(QBrush(QColor(120,0,0)));
            }
        }
    }

}
