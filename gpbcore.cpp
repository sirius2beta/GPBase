#include "gpbcore.h"

GPBCore::GPBCore(QObject *parent, QString config)
    : QObject{parent},
      _config(config)
{
    settings = new QSettings("Ezosirius", "GPlayer_v1",this);

}

GPBCore::~GPBCore(){
    delete _boatSetting;
    delete _boatManager;
    delete _sensorWidget;
}

void GPBCore::init(){
    _boatManager = new BoatManager(this, this);
    _networkManager = new NetworkManager(this, this);

    _boatSetting = new BoatSetting();
    connect(_boatSetting, &BoatSetting::AddBoat, this, &GPBCore::onNewBoat);

    connect(_networkManager, &NetworkManager::sensorMsg, _boatSetting, &BoatSetting::onMsg);
    connect(_boatSetting, &BoatSetting::connectionTypeChanged, this, &GPBCore::onConnectionTypeChanged);
    _boatSetting->setconfig(_config);
    _boatSetting->initSettings(_boatManager);
    qDebug()<<"ck03";
    _sensorWidget = new SensorWidget();
    _sensorWidget->setBoatList(_boatManager);
    connect(_sensorWidget, &SensorWidget::sendMsg, _networkManager, &NetworkManager::sendMsg);
    qDebug()<<"ck04";
    _networkManager->init();
}

void GPBCore::onNewBoat(BoatItem* newboat)
{
    qDebug()<<"ck1-1";

    _primaryHeartBeat = new HeartBeat(newboat, 50006, true, newboat);
    _primaryHeartBeat->HeartBeatLoop();
    _secondaryHeartBeat = new HeartBeat(newboat, 50006, false, newboat);
    _secondaryHeartBeat->HeartBeatLoop();

    connect(newboat, &BoatItem::connected, _boatSetting, &BoatSetting::onConnected);
    connect(newboat, &BoatItem::disconnected, _boatSetting, &BoatSetting::onDisonnected);
    connect(newboat, &BoatItem::connected, this, &GPBCore::onConnected);
    connect(newboat, &BoatItem::disconnected,this, &GPBCore::onDisonnected);

    connect(_networkManager, &NetworkManager::AliveResponse, _primaryHeartBeat, &HeartBeat::alive);
    connect(_primaryHeartBeat, &HeartBeat::sendMsg, _networkManager, &NetworkManager::sendMsg);
    connect(newboat, &BoatItem::IPChanged, _primaryHeartBeat, &HeartBeat::onChangeIP);

    connect(_networkManager, &NetworkManager::AliveResponse, _secondaryHeartBeat, &HeartBeat::alive);
    connect(_secondaryHeartBeat, &HeartBeat::sendMsg, _networkManager, &NetworkManager::sendMsg);
    connect(newboat, &BoatItem::IPChanged, _secondaryHeartBeat, &HeartBeat::onChangeIP);

}



void GPBCore::onConnectionTypeChanged()
{
    emit connectionChanged();
}

void GPBCore::onConnected(int ID, bool isprimary)
{

    BoatItem* boat = _boatManager->getBoatbyID(ID);
    if(boat != 0){
        if(isprimary){
            boat->setCurrentIP(boat->PIP());
            emit connectionChanged();
        }else{
            if(boat->currentIP() != boat->PIP()){
                boat->setCurrentIP(boat->SIP());
                emit connectionChanged();
            }
        }
    }


}

void GPBCore::onDisonnected(int ID, bool isprimary)
{

    BoatItem* boat = _boatManager->getBoatbyID(ID);
    if(boat != 0){
        if(isprimary){
            boat->setCurrentIP(boat->SIP());
            emit connectionChanged();
        }

    }



}
