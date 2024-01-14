#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <QObject>
#include <QHostAddress>

#include "boat.h"
#include "boatmanager.h"

class HeartBeat : public QObject
{
    Q_OBJECT
public:
    explicit HeartBeat(QObject *parent = nullptr);
    HeartBeat(Boat* boat, BoatManager* boatList, QString PC_ip, int port, bool isPrimary, QObject *parent = nullptr);
    void HeartBeatLoop(); //工作函数
    ~HeartBeat();
    void setPCIP(QString PC_ip);

signals:
    void sendMsg(QHostAddress addr, char topic, QByteArray command);
    void connected(int ID, bool isprimary);
    void disconnected(int ID, bool isprimary);

public slots:
    void beat();
    void checkAlive();
    void alive(QString ip);
    void resetBoatName(int boatIndex, QString newname);
    void onChangeIP(Boat *boat, bool isPrimary);
    void onDeleteBoat(QString boatname);

private:
    QString boatIP;
    bool run;
    QTimer *heartBeatTimer;
    QTimer *checkAliveTimer;
    int boatPort;
    QString PCIP;
    bool isAlive;
    bool isHearBeatLoop;
    bool primary;
    Boat* boat;
    BoatManager* boatList;
};

#endif // HEARTBEAT_H
