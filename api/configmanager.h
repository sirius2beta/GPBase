#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QXmlStreamReader>

#include <QStandardItemModel>

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr);

signals:
protected:
    void readSensorTypes();
private:
    QXmlStreamReader reader;
    QStandardItemModel* s
};

#endif // CONFIGMANAGER_H
