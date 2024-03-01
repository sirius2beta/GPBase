#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QXmlStreamReader>

#include <QStandardItemModel>

#include "dnvalue.h"

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);
    QVector<DNValue> sensorTypeList() { return _sensorTypeList; };
signals:
protected:
    void readSensorTypes();
private:
    QXmlStreamReader reader;
    QStandardItemModel* s;
    QVector<DNValue> _sensorTypeList;
};

#endif // CONFIGMANAGER_H
