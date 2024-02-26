#include "networkmanagerControl.h"

NetworkManagerControl::NetworkManagerControl(QObject *parent, BoatManager* boatList)
    : QObject{parent}
{
    this->boatList = boatList;
    serverSocket = new QUdpSocket(this);
    clientSocket = new QUdpSocket(this);
    clientSocket->bind(50008,QUdpSocket::ShareAddress);

}

void NetworkManagerControl::init()
{
    connect(clientSocket,&QUdpSocket::readyRead,this, &NetworkManagerControl::onUDPMsg);
}

void NetworkManagerControl::sendMsg(QHostAddress addr, char topic, QByteArray command)
{
    QByteArray cmd;
    cmd.resize(1);
    cmd[0] = topic;
    cmd.append(command);
    serverSocket->writeDatagram(cmd,cmd.size(), addr, 50006);
}

void NetworkManagerControl::onUDPMsg()
{
    while(clientSocket->hasPendingDatagrams()){
        QByteArray data;
        QHostAddress addr;
        QString ip;
        data.resize(clientSocket->pendingDatagramSize());
        clientSocket->readDatagram(data.data(),data.size(),&addr);

        char topic = data[0];
        data.remove(0,1);

        ip = QHostAddress(addr.toIPv4Address()).toString();


        QStringList dataList = QString(data).split(' ');

        QString message;
        if(data.split(' ').size() >1){
            message = data.split(' ')[1];
        }
        if(topic == HEARTBEAT){
            int ID = int(data[0]);
            BoatItem* boat = boatList->getBoatbyID(ID);
            if( boat != 0){

                emit AliveResponse(ip);
            }


        }else if(topic == FORMAT){
            int ID = int(data[0]);
            QString format = data.remove(0,1);
            qDebug()<<"MainWindow call from FORMAT, boat ID:"<<ID;
            qDebug()<<format;
            if(format != ""){
                emit setFormat(ID, format.split('\n'));
            }

        }else if(topic == SENSOR){
            qDebug()<<"sensor";
            emit sensorMsg(data);
        }
        const QString content = QLatin1String(" Received Topic: ")
                    + topic
                    + QLatin1String(" Message: ")
                    + message
                    + QLatin1Char('\n');
        //qDebug() << content;
    }
}
