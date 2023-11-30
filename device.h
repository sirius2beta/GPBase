#ifndef DEVICE_H
#define DEVICE_H
#include <QWidget>
#include <QDebug>

class Component
{
public:
    Component();
    Component(const Component& c);
    int periID;
    int comID;
    int type;
    QList<int> pinList;
    QString setting;

};

class Device
{
public:
    Device();
    Device(const Device &d);
    ~Device();

    int ID;
    QString deviceName;
    int type;
    QString settings;
    QList<int> pinUIDList;
    int boardofPin(int index);
    int pinIDofPin(int index);
    int addPin(int boardID, int pinID);
    int addPin(int UID);
    QString typeName();

    bool operator== (const Device& lDev);

    enum DEV {SENSOR = 0, SERVO = 1, STEPPER=2};

};

class Peripheral{
public:
    Peripheral();
    Peripheral(const Peripheral &p);
    QList<int> pinSelected;
    int ID;
    QString boardName;
    QString boardType;
};

#endif // DEVICE_H
