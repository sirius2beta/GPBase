#include <QTimer>
#include <QDebug>

#include "heartbeat.h"

#include "QTypes.h"

HeartBeat::HeartBeat(QObject *parent): QObject(parent)
{

}

HeartBeat::HeartBeat(QString boatname, int ID, QString PC_ip, QString Boat_ip, int port, bool isPrimary, QObject *parent): QObject(parent)
{
    boatName = boatname;
    boatIP = Boat_ip;
    boatID = ID;
    PCIP = PC_ip;
    boatPort = port;
    isAlive = false;
    isHearBeatLoop = false;
    primary = isPrimary;
    heartBeatTimer = new QTimer(this);
    checkAliveTimer = new QTimer(this);
    connect(heartBeatTimer, &QTimer::timeout, this, &HeartBeat::beat);
    connect(checkAliveTimer,&QTimer::timeout, this, &HeartBeat::checkAlive);
    qDebug()<<"HB init: "<<"ID "<< boatID;
}

HeartBeat::~HeartBeat()
{
    run = false;
}

void HeartBeat::resetBoatName(QString boatname, QString newname)
{
    qDebug()<<"HeartBeat: "<<boatname+" >> "+boatName+" >> "+newname;
    if(boatname == boatName){
        boatName = newname;
        if(!isHearBeatLoop){
            checkAliveTimer->stop();
            HeartBeatLoop();
        }
    }
}

void HeartBeat::onChangeIP(QString boatname, QString PIP, QString SIP)
{
    if(boatname == boatName){
        if(primary){
            boatIP = PIP;
        }else{
            boatIP = SIP;
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

    if(boatName == boatname){
        this->deleteLater();
    }

}

void HeartBeat::HeartBeatLoop()
{
    //qDebug()<<"HeartBeat "<<", boatname:"<<boatName<<", boat ip:"<<boatIP<<", HB loop";
    QByteArray cmd_bytes;

    if(primary){
        //cmd =  PCIP+QString(" ")+boatName+QString(" P");
        quint32 addr_raw = QHostAddress(PCIP).toIPv4Address();
        QByteArray addr_bytes = QByteArray::fromRawData(reinterpret_cast<const char *>(&addr_raw), sizeof(addr_raw));
        cmd_bytes = addr_bytes;
        cmd_bytes.resize(6);
        cmd_bytes[4] = boatID;
        cmd_bytes[5] = 'P';
    }else{
        //cmd =  PCIP+QString(" ")+boatName+QString(" S");
        quint32 addr_raw = QHostAddress(PCIP).toIPv4Address();
        QByteArray addr_bytes = QByteArray::fromRawData(reinterpret_cast<const char *>(&addr_raw), sizeof(addr_raw));
        cmd_bytes = addr_bytes;
        cmd_bytes.resize(6);
        cmd_bytes[4] = boatID;
        cmd_bytes[5] = 'S';
    }
    emit sendMsg(QHostAddress(boatIP), char(HEARTBEAT),cmd_bytes);
    heartBeatTimer->start(1000);
    isHearBeatLoop = true;
    isAlive = false;
    emit disconnected(boatID,primary);
}


void HeartBeat::alive(QString ip)
{
    if(ip == boatIP){
        if(isHearBeatLoop == false){
            //qDebug()<<"HeartBeat: alive from "<<ip;
            isAlive = true;
        }else{
            // Enter alive loop
            qDebug()<<"HeartBeat "<<", boatname:"<<boatName<<", boat ip:"<<ip<<", alive loop";
            heartBeatTimer->stop();
            checkAliveTimer->start(1500);
            isHearBeatLoop = false;
            qDebug()<<"Start alive loop";
            emit connected(boatID,primary);
            emit sendMsg(QHostAddress(ip), char(FORMAT), QString("q").toLocal8Bit());
            emit sendMsg(QHostAddress(boatName), char(SENSOR), QString("d").toLocal8Bit());
            qDebug()<<"HeartBeat boatname:"<<boatName;

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
        //cmd =  PCIP+QString(" ")+boatName+QString(" P");
        quint32 addr_raw = QHostAddress(PCIP).toIPv4Address();
        QByteArray addr_bytes = QByteArray::fromRawData(reinterpret_cast<const char *>(&addr_raw), sizeof(addr_raw));
        cmd_bytes = addr_bytes;
        cmd_bytes.resize(6);
        cmd_bytes[4] = boatID;
        cmd_bytes[5] = 'P';
    }else{
        //cmd =  PCIP+QString(" ")+boatName+QString(" S");
        quint32 addr_raw = QHostAddress(PCIP).toIPv4Address();
        QByteArray addr_bytes = QByteArray::fromRawData(reinterpret_cast<const char *>(&addr_raw), sizeof(addr_raw));
        cmd_bytes = addr_bytes;
        cmd_bytes.resize(6);
        cmd_bytes[4] = boatID;
        cmd_bytes[5] = 'S';
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
