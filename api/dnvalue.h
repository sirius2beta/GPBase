#ifndef DNVALUE_H
#define DNVALUE_H

#include <QObject>
#include <QVariant>
#include "dnmetadata.h"

class DNValue : public QObject
{
    Q_OBJECT
public:
    DNValue(QObject *parent = nullptr);
    DNValue(int sensorType, QString name, DNMetaData::ValueType_t type, QObject *parent = nullptr);
    DNValue(const DNValue& other, QObject *parent = nullptr);
    const DNValue& operator = (const DNValue& other);
    QString name(){ return _name; }
    int SensorType() { return _sensorType; }
    QVariant data() { return _rawValue; }
    QVariant dataType() { return _type; }
    ~DNValue();

private:
    QString _name;
    int _sensorType;
    QVariant _rawValue;
    DNMetaData::ValueType_t _type;
};

#endif // DNVALUE_H
