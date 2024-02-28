#ifndef BOATITEM_H
#define BOATITEM_H

#include <QObject>
#include "device.h"
#include "boatitem.h"

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
        return _currentIP;
    };
    int OS(void) {    return _OS;   };
    int linkType() { return _linkType; }


    void setName(QString name);
    void setID(int ID);
    void setPIP(QString PIP);
    void setSIP(QString SIP);
    void setOS(int OS);
    void setConnectionPriority(int connectionType);
    void setPrimaryConnected(bool connected){ primaryConnected = connected; }
    void setSecondaryConnected(bool connected){ secondaryConnected = connected; }

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
    void connected(int ID, bool isPrimary);
    void disconnected(int ID, bool isPrimary);
    void connectionChanged(int ID);
private:
    QString _name;
    int _ID;
    QString _PIP;
    QString _SIP;
    QString _currentIP;
    int _OS;
    bool primaryConnected;
    bool secondaryConnected;
    int _connectionPriority;
    int _linkType;


    HeartBeat* _primaryHeartBeat;
    HeartBeat* _secondaryHeartBeat;

};

#endif // BOATITEM_H
