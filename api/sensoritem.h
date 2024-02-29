#ifndef SENSORITEM_H
#define SENSORITEM_H

#include <QObject>

class SensorItem : public QObject
{
    Q_OBJECT
public:
    explicit SensorItem(QObject *parent = nullptr);
    int boatID() { return _boatID; }
    int sensorType() { return _sensorType; }
    QString name() { return _name; }

    void setBoatID(int ID);
    void setSensorType(int type);
    void setName(QString name);
    void setData(int dataType, QByteArray data);

signals:
    void BoatIDSet(int ID);
    void sensorTypeSet(int type);
    void setText(QString text);
    void nameSet(QString name);
private:
    QByteArray _data;
    int _boatID;
    int _sensorType;
    QString _name;

};

#endif // SENSORITEM_H
