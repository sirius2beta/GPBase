#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QUdpSocket>
#include "boatmanager.h"

class GPBCore;

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr, GPBCore* core = nullptr);
    void init();

signals:
    void AliveResponse(QString ip, int ID);
    void setFormat(int ID, QStringList format);
    void sensorMsg(QByteArray data);
public slots:
    void sendMsg(QHostAddress addr, char topic, QByteArray command);

protected slots:
    void onUDPMsg();
private:
    QUdpSocket *serverSocket;
    QUdpSocket *clientSocket;
    GPBCore* _core;
};

#endif // NETWORKMANAGER_H
