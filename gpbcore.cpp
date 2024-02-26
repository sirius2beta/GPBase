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
}

void GPBCore::init(){
    _networkManager = new NetworkManager(this, this);
    _boatManager = new BoatManager(this, this);
    _videoManager = new DNVideoManager(this, this);

    _boatSetting = new BoatSetting();

    connect(_videoManager, &DNVideoManager::sendMsg, _networkManager, &NetworkManager::sendMsg);
    connect(_networkManager, &NetworkManager::setFormat, _videoManager, &DNVideoManager::setVideoFormat);
    connect(_networkManager, &NetworkManager::sensorMsg, _boatSetting, &BoatSetting::onMsg);
    connect(_boatSetting, &BoatSetting::connectionTypeChanged, this, &GPBCore::onConnectionTypeChanged);
    connect(_boatManager, &BoatManager::BoatAdded, _videoManager, &DNVideoManager::onBoatAdded);
    _boatSetting->setconfig(_config);
    _boatSetting->initSettings(_boatManager);
    _videoManager->init();
    _networkManager->init();

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
            qDebug()<<"GPBCore onDisconnected "<<QString::number(ID);
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
