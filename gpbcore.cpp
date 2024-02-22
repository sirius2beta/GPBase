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
    _networkManager = new NetworkManager(this, this);
    _boatManager = new BoatManager(this, this);


    _boatSetting = new BoatSetting();
    connect(_boatManager, &BoatManager::boatAdded, this, &GPBCore::onNewBoat);

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
