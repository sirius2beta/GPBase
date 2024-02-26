#ifndef BOATITEM_H
#define BOATITEM_H

#include <QObject>
#include "device.h"

class HeartBeat;

class BoatItem : public QObject
{
    Q_OBJECT
public:
    explicit BoatItem(QObject *parent = nullptr);
    ~BoatItem();
    QString name(void) {    return _name;   };
    int ID(void) {    return _ID; };
    QString PIP(void) {    return _PIP;    };
    QString SIP(void) {    return _SIP;    };
    QString currentIP(void) {
        if(!primaryConnected){
            return _SIP;
        }else{
            return _PIP;
        }
    };
    int OS(void) {    return _OS;   };


    void setName(QString name);
    void setID(int ID);
    void setPIP(QString PIP);
    void setSIP(QString SIP);
    void setCurrentIP(QString currentIP);
    void setOS(int OS);
    Device& getDevbyID(int ID);
    Peripheral getPeriperalbyID(int ID);
    void connect(bool isPrimary);
    void disconnect(bool isPrimary);

    QList<Peripheral> peripherals;
    QList<Device> devices;
signals:
    void nameChanged(int ID, QString name);
    void IDChanged(int ID);
    void IPChanged(int ID, bool isPrimary);
    void CurrentIPChanged(QString curretIP);
    void connected(int ID, bool isPrimary);
    void disconnected(int ID, bool isPrimary);
    void connectionChanged();
private:
    QString _name;
    int _ID;
    QString _PIP;
    QString _SIP;
    QString _currentIP;
    int _OS;
    bool primaryConnected;

    HeartBeat* _primaryHeartBeat;
    HeartBeat* _secondaryHeartBeat;

};

#endif // BOATITEM_H
