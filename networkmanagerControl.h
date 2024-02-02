#ifndef NETWORKMANAGERCONTROL_H
#define NETWORKMANAGERCONTROL_H

#include <QObject>
#include <QUdpSocket>
#include "boatmanager.h"
#include "gtypes.h"

class NetworkManagerControl : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManagerControl(QObject *parent = nullptr, BoatManager* boatList = 0);
    void init();

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

#endif // NETWORKMANAGERCONTROL_H
