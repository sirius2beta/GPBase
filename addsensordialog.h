#ifndef ADDSENSORDIALOG_H
#define ADDSENSORDIALOG_H

#include <QDialog>
#include "sensoritem.h"

class GPBCore;

namespace Ui {
class AddSensorDialog;
}

class AddSensorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSensorDialog(QWidget *parent = nullptr, GPBCore* core = nullptr);
    ~AddSensorDialog();
    void accept();
signals:
    void comit(SensorItem* sensorItem);

private:
    Ui::AddSensorDialog *ui;
    GPBCore* _core;
};

#endif // ADDSENSORDIALOG_H
