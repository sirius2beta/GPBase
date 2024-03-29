﻿#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

#include <QDialog>
#include <QStandardItem>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "boatmanager.h"
#include "setpindialog.h"
#include "device.h"
#include "boatitem.h"

namespace Ui {
class AddDeviceDialog;
}

class AddDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDeviceDialog(QWidget *parent = nullptr);
    ~AddDeviceDialog();
    void setPeripheralModel(QStandardItemModel* pm, BoatItem* boat);
    QList<Peripheral> getPeripherals();
    Device getDevice();
    QString getDevName();
    QWidget *addDevNameField();
    QWidget *addPinSelectField(QString name);

signals:
    void addDev();
protected slots:
    void onAddSensor();
    void onAddServo();
    void onAddStepper();
    void onSelectPins();
    void onDevNameChanged(const QString& n);
private:
    Ui::AddDeviceDialog *ui;
    BoatItem* thisBoat;
    QStandardItemModel* peripheralItemModel;
    QList<Peripheral> PeripheralCache;
    QWidget* backgrounWidget;
    int currentMode;
    Device newDev;
    QList<Peripheral> newPeripherals;
    QList<Peripheral> oldPeripherals;
    QString devName;
    QLineEdit* selectedPinLineEdit;
    QList<QLineEdit*> pinLineEditList;
    QList<QPushButton*> pinPushButton;
    QList<QList<int>> pinCurrentSelected;


};

#endif // ADDDEVICEDIALOG_H
