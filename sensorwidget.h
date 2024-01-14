#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QMap>
#include <QDebug>
#include <QHostAddress>
#include "sensorlabel.h"
#include "boatmanager.h"

namespace Ui {
class sensorWidget;
}

class sensorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit sensorWidget(QWidget *parent = nullptr);
    ~sensorWidget();
    void setBoatList(BoatManager* boatlist);
public slots:
    void addWidget(QString name, int BoatID, int DeviceID);
    void info_button_pushed();
    void subscribeSensor(int BoatID, int BoardID, int PinID, char unit);

signals:

    void sendMsg(QHostAddress addr, char topic, QByteArray command);

private:
    Ui::sensorWidget *ui;
    int connection_counter;
    int costumSensorCount;
    BoatManager *boatList;
    QMap<int,SensorLabel*> labelMap;

};

#endif // SENSORWIDGET_H
