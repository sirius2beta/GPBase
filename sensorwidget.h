#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QMap>
#include <QDebug>
#include <QHostAddress>
#include <QSound>
#include "sensorlabel.h"
#include "boatmanager.h"
#include "sensormanager.h"


class GPBCore;

namespace Ui {
class sensorWidget;
}

class SensorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SensorWidget(QWidget *parent = nullptr, GPBCore *core = nullptr);
    ~SensorWidget();
public slots:
    void addWidget(SensorItem *sensorItem);
    void info_button_pushed();
    void subscribeSensor(int BoatID, int BoardID, int PinID, char unit);
    void onAddSensorButtonClicked();

signals:

private:
    Ui::sensorWidget *ui;
    int connection_counter;
    int costumSensorCount;
    GPBCore* _core;
    SensorManager* _sensorManager;
    QMap<int,SensorLabel*> labelMap;

};

#endif // SENSORWIDGET_H
