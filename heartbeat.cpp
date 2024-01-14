#include <QTimer>
#include <QDebug>

#include "heartbeat.h"

#include "QTypes.h"

HeartBeat::HeartBeat(QObject *parent): QObject(parent)
{

}

HeartBeat::HeartBeat(Boat* boat, QString PC_ip, int port, bool isPrimary, QObject *parent): QObject(parent)
{

    PCIP = PC_ip;
    boatPort = port;
    isAlive = false;
    isHearBeatLoop = false;
    primary = isPrimary;
    this->boat = boat;
    if(primary){
        boatIP = boat->PIP;
    }else{
        boatIP = boat->SIP;
    }
    heartBeatTimer = new QTimer(this);
    checkAliveTimer = new QTimer(this);
    connect(heartBeatTimer, &QTimer::timeout, this, &HeartBeat::beat);
    connect(checkAliveTimer,&QTimer::timeout, this, &HeartBeat::checkAlive);
    qDebug()<<"HB init: "<<"ID "<< boat->ID;

}

HeartBeat::~HeartBeat()
{
    run = false;
}

void HeartBeat::resetBoatName(int boatID, QString newname)
{
    //qDebug()<<"HeartBeat: "<<boatname+" >> "+boatName+" >> "+newname;
    if(boatID == boat->ID){
        if(!isHearBeatLoop){
            checkAliveTimer->stop();
            HeartBeatLoop();
        }
    }
}

void HeartBeat::onChangeIP(QString boatname, QString PIP, QString SIP)
{

    if(boatname == boat->boatName){
        if(primary){
            boat->PIP = PIP;
        }else{
            boat->SIP = SIP;
        }
        if(primary){
            boatIP = boat->PIP;
        }else{
            boatIP = boat->SIP;
        }
        if(!isHearBeatLoop){
            checkAliveTimer->stop();
            HeartBeatLoop();
        }
    }
}

void HeartBeat::onPCPIPChanged(QString IP)
{
        if(primary){
            PCIP = IP;
            if(!isHearBeatLoop){
                checkAliveTimer->stop();
                HeartBeatLoop();
            }
        }

}

void HeartBeat::onPCSIPChanged(QString IP)
{
        if(!primary){
            PCIP = IP;
            if(!isHearBeatLoop){
                checkAliveTimer->stop();
                HeartBeatLoop();
            }
        }

}

void HeartBeat::onDeleteBoat(QString boatname)
{

    if(boatname == boat->boatName){
        this->deleteLater();
    }

}

void HeartBeat::HeartBeatLoop()
{
    qDebug()<<"HeartBeat "<<", boatname:"<<boat->boatName<<", boat ip:"<<boatIP<<", HB loop";

    beat();
    heartBeatTimer->start(1000);
    isHearBeatLoop = true;
    isAlive = false;
    emit disconnected(boat->ID,primary);

}


void HeartBeat::alive(QString ip)
{
    if(ip == boatIP){
        if(isHearBeatLoop == false){
            //qDebug()<<"HeartBeat: alive from "<<ip;
            isAlive = true;
        }else{
            // Enter alive loop
            qDebug()<<"HeartBeat "<<", boatname:"<<boat->boatName<<", boat ip:"<<ip<<", alive loop";
            heartBeatTimer->stop();
            checkAliveTimer->start(1500);
            isHearBeatLoop = false;
            qDebug()<<"Start alive loop";
            emit connected(boat->ID,primary);
            emit sendMsg(QHostAddress(ip), char(FORMAT), QString("q").toLocal8Bit());
            emit sendMsg(QHostAddress(boat->boatName), char(SENSOR), QString("d").toLocal8Bit());
            qDebug()<<"HeartBeat boatname:"<<boat->boatName;

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
        cmd_bytes[0] = boat->ID;
        cmd_bytes[1] = 'P';

    }else{
        cmd_bytes.resize(2);
        cmd_bytes[0] = boat->ID;
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

void HeartBeat::setPCIP(QString PC_ip)
{
    PCIP = PC_ip;
    if(checkAliveTimer->isActive()){
        checkAliveTimer->stop();
        isAlive = false;
        HeartBeatLoop();
    }

}
