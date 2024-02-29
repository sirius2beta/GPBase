#ifndef SENSORLABEL_H
#define SENSORLABEL_H

#include <QWidget>
#include "sensoritem.h"

namespace Ui {
class SensorLabel;
}

class SensorLabel : public QWidget
{
    Q_OBJECT

public:
    explicit SensorLabel(QWidget *parent = nullptr);
    ~SensorLabel();
    void setSensorItem(SensorItem* sensorItem);
public slots:
    void setValue(QString value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::SensorLabel *ui;
    SensorItem* _sensorItem;
};

#endif // SENSORLABEL_H
