#ifndef SENSORLABEL_H
#define SENSORLABEL_H

#include <QWidget>
#include <QSound>
#include "sensoritem.h"

namespace Ui {
class SensorLabel;
}

class SensorLabel : public QWidget
{
    Q_OBJECT

public:
    explicit SensorLabel(QWidget *parent = nullptr, SensorItem* sensorItem = nullptr);
    ~SensorLabel();
    void setSensorItem(SensorItem* sensorItem);
public slots:
    void onSetText(QString text);
    void onDisableAlarmSound();
protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::SensorLabel *ui;
    SensorItem* _sensorItem;
    bool _isPlaying;
    bool _disableAlarmSound;
    QSound* alarm;
};

#endif // SENSORLABEL_H
