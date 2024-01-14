#include "gpbcore.h"

GPBCore::GPBCore(QObject *parent, QString config)
    : QObject{parent},
      _config(config)
{
    settings = new QSettings("Ezosirius", "GPlayer_v1",this);

    _boatList = new BoatManager;
    _boatSetting = new BoatSetting();
    connect(_boatSetting, &BoatSetting::AddBoat, this, &GPBCore::onNewBoat);
    connect(_boatSetting, &BoatSetting::sendMsg, _networkManager, &NetworkManager::sendMsg);
    connect(_networkManager, &NetworkManager::sensorMsg, _boatSetting, &BoatSetting::onMsg);
    connect(_boatSetting, &BoatSetting::connectionTypeChanged, this, &GPBCore::onConnectionTypeChanged);
    _boatSetting->setconfig(_config);
    _boatSetting->initSettings(_boatList);
}

GPBCore::~GPBCore(){
    delete _boatSetting;
    delete _boatList;
}

void GPBCore::onNewBoat(Boat* newboat)
{
    _primaryHeartBeat = new HeartBeat(newboat, _boatList, 50006, true, this);
    qDebug()<<"MainWindow::onNewBoat ck1";
    _primaryHeartBeat->HeartBeatLoop();
    qDebug()<<"MainWindow::onNewBoat ck2";
    _secondaryHeartBeat = new HeartBeat(newboat, _boatList, 50006, false, this);
    _secondaryHeartBeat->HeartBeatLoop();
    connect(_networkManager, &NetworkManager::AliveResponse, _primaryHeartBeat, &HeartBeat::alive);
    connect(_primaryHeartBeat, &HeartBeat::sendMsg, _networkManager, &NetworkManager::sendMsg);
    connect(_primaryHeartBeat, &HeartBeat::connected, _boatSetting, &BoatSetting::onConnected);
    connect(_primaryHeartBeat, &HeartBeat::disconnected, _boatSetting, &BoatSetting::onDisonnected);
    connect(_primaryHeartBeat, &HeartBeat::connected, this, &GPBCore::onConnected);
    connect(_primaryHeartBeat, &HeartBeat::disconnected, this, &GPBCore::onDisonnected);
    connect(_boatSetting, &BoatSetting::changeBoatName, _primaryHeartBeat, &HeartBeat::resetBoatName);
    connect(_boatSetting, &BoatSetting::ChangeIP, _primaryHeartBeat, &HeartBeat::onChangeIP);
    connect(_boatSetting, &BoatSetting::deleteBoat, _primaryHeartBeat, &HeartBeat::onDeleteBoat);

    connect(_networkManager, &NetworkManager::AliveResponse, _secondaryHeartBeat, &HeartBeat::alive);
    connect(_secondaryHeartBeat, &HeartBeat::sendMsg, _networkManager, &NetworkManager::sendMsg);
    connect(_secondaryHeartBeat, &HeartBeat::connected, _boatSetting, &BoatSetting::onConnected);
    connect(_secondaryHeartBeat, &HeartBeat::disconnected, _boatSetting, &BoatSetting::onDisonnected);
    connect(_secondaryHeartBeat, &HeartBeat::connected, this, &GPBCore::onConnected);
    connect(_secondaryHeartBeat, &HeartBeat::disconnected, this, &GPBCore::onDisonnected);
    connect(_boatSetting, &BoatSetting::changeBoatName, _secondaryHeartBeat, &HeartBeat::resetBoatName);
    connect(_boatSetting, &BoatSetting::ChangeIP, _secondaryHeartBeat, &HeartBeat::onChangeIP);
    connect(_boatSetting, &BoatSetting::deleteBoat, _secondaryHeartBeat, &HeartBeat::onDeleteBoat);
}



void GPBCore::onConnectionTypeChanged()
{
    emit connectionChanged();
}

void GPBCore::onConnected(int ID, bool isprimary)
{

    Boat* boat = _boatList->getBoatbyID(ID);
    if(boat != 0){
        if(isprimary){
            boat->CurrentIP = boat->PIP;
            emit connectionChanged();
        }else{
            if(boat->CurrentIP != boat->PIP){
                boat->CurrentIP = boat->SIP;
                emit connectionChanged();
            }
        }
    }


}

void GPBCore::onDisonnected(int ID, bool isprimary)
{

    Boat* boat = _boatList->getBoatbyID(ID);
    if(boat != 0){
        if(isprimary){
            boat->CurrentIP = boat->SIP;
            emit connectionChanged();
        }

    }



}
