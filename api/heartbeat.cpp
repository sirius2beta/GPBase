#include <QTimer>
#include <QDebug>

#include "heartbeat.h"

#include "QTypes.h"
#include "networkmanager.h"
#include "gpbcore.h"

HeartBeat::HeartBeat(QObject *parent, GPBCore *core): QObject(parent)
{
    _core = core;
}

HeartBeat::HeartBeat(BoatItem* boat, int port, bool isPrimary, QObject *parent, GPBCore *core): QObject(parent)
{

    _core = core;
    boatPort = port;
    isAlive = false;
    isHearBeatLoop = false;
    primary = isPrimary;
    this->boat = boat;
    if(primary){
        boatIP = boat->PIP();
    }else{
        boatIP = boat->SIP();
    }
    heartBeatTimer = new QTimer(this);
    checkAliveTimer = new QTimer(this);
    connect(heartBeatTimer, &QTimer::timeout, this, &HeartBeat::beat);
    connect(checkAliveTimer,&QTimer::timeout, this, &HeartBeat::checkAlive);
    connect(_core->networkManager(), &NetworkManager::AliveResponse, this, &HeartBeat::alive);
    connect(this, &HeartBeat::sendMsg, _core->networkManager(), &NetworkManager::sendMsg);

    qDebug()<<"HeartBeat: initiated (ID: "<< boat->ID()<<")";

}

HeartBeat::~HeartBeat()
{
    run = false;
}



void HeartBeat::onChangeIP(int ID, bool isPrimary)
{

    if(primary == isPrimary){
        if(primary){
            boatIP = boat->PIP();
        }else{
            boatIP = boat->SIP();
        }
        if(!isHearBeatLoop){
            checkAliveTimer->stop();
            HeartBeatLoop();
        }
    }


}


void HeartBeat::onDeleteBoat(QString boatname)
{

    if(boatname == boat->name()){
        this->deleteLater();
    }

}

void HeartBeat::HeartBeatLoop()
{
    //qDebug()<<"HeartBeat::HeartBeatLoop started ("<<boat->name()<<", "<<boatIP<<")";

    beat();
    heartBeatTimer->start(1000);
    isHearBeatLoop = true;
    isAlive = false;

    boat->disconnect(primary);



}


void HeartBeat::alive(QString ip, int ID)
{
    if((ip == boatIP) && (ID == boat->ID())){
        if(isHearBeatLoop == false){

            isAlive = true;
        }else{
            // Enter alive loop
            //qDebug()<<"HeartBeat::AliveLoop started ("<<boat->name()<<", "<<ip<<")";
            heartBeatTimer->stop();
            checkAliveTimer->start(2000);
            isHearBeatLoop = false;
            boat->connect(primary);
            emit sendMsg(QHostAddress(ip), char(FORMAT), QString("q").toLocal8Bit());
            emit sendMsg(QHostAddress(boat->name()), char(SENSOR), QString("d").toLocal8Bit());
            qDebug()<<"HeartBeat boatname:"<<boat->name();

        }
    }else{
        //Deposit other boat's message;
    }
}

void HeartBeat::beat()
{
    //QString cmd;
    QByteArray cmd_bytes;

    if(primary){
        cmd_bytes.resize(2);
        cmd_bytes[0] = boat->ID();
        cmd_bytes[1] = 'P';

    }else{
        cmd_bytes.resize(2);
        cmd_bytes[0] = boat->ID();
        cmd_bytes[1] = 'S';
    }
    emit sendMsg(QHostAddress(boatIP), char(HEARTBEAT),cmd_bytes);
}

void HeartBeat::checkAlive()
{
    if(isAlive == false){
        checkAliveTimer->stop();
        HeartBeatLoop();
    }
    isAlive = false;
}

