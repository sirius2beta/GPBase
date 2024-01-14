#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QUdpSocket>
#include "boatmanager.h"

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr, BoatManager* boatList = 0);

signals:
    void AliveResponse(QString ip);
    void setFormat(int ID, QStringList format);
    void sensorMsg(QByteArray data);
public slots:
    void sendMsg(QHostAddress addr, char topic, QByteArray command);

protected slots:
    void onUDPMsg();
private:
    QUdpSocket *serverSocket;
    QUdpSocket *clientSocket;
    BoatManager* boatList;

};

#endif // NETWORKMANAGER_H
