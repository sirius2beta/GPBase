#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <QObject>
#include <QHostAddress>

#include "boat.h"

class HeartBeat : public QObject
{
    Q_OBJECT
public:
    explicit HeartBeat(QObject *parent = nullptr);
    HeartBeat(Boat* boat, QString PC_ip, int port, bool isPrimary, QObject *parent = nullptr);
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
    void resetBoatName(int boatID, QString newname);
    void onChangeIP(QString boatname, QString PIP, QString SIP);
    void onPCPIPChanged(QString IP);
    void onPCSIPChanged(QString IP);
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
};

#endif // HEARTBEAT_H
