#ifndef SENSORITEM_H
#define SENSORITEM_H

#include <QObject>
#include "dnvalue.h"

class SensorItem : public QObject
{
    Q_OBJECT
public:
    explicit SensorItem(QObject *parent = nullptr);
    int boatID() { return _boatID; }
    DNValue value() { return _value; }
    QString name() { return _value.name(); }
    QVariant maxAlarmValue() { return _maxAlarmValue; }
    bool isEnableMaxAlarm() { return _enableMaxAlarm; }

    void setBoatID(int ID);
    void setName(QString name);
    void setValue(DNValue value);
    void setMaxAlarmValue(QVariant value){ _maxAlarmValue = value; };
    void enableMaxAlarm(bool enable) { _enableMaxAlarm = enable; };

signals:
    void BoatIDSet(int ID);
    void textSet(QString text);
    void nameSet(QString name);
private:
    DNValue _value;
    QVariant _maxAlarmValue;
    bool _enableMaxAlarm;
    int _boatID;

};

#endif // SENSORITEM_H
