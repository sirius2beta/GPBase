﻿#ifndef SETPINDIALOG_H
#define SETPINDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QList>

#include "comboboxdelegate.h"
#include "boatmanager.h"

namespace Ui {
class SetPinDialog;
}

class SetPinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetPinDialog(QWidget *parent = nullptr);
    ~SetPinDialog();
    void setPeripheralModel(QStandardItemModel* pm, BoatItem* boat);
    QList<Peripheral> getPeripherals();
    const Device& getDevice();
    void setType(int type);
    void setPeripherals(const QList<Peripheral>& peripherals);
    void setDev(const Device& Dev);
    void setMaxNum(int num);
    void setIndex(int i);
    int getIndex();
    QList<int> getpinCurrentSelected();
protected slots:
    void onBoardSelected(const QModelIndex &index);
    void onDeviceItemChanged(QStandardItem *item);
private:
    BoatItem* thisBoat;
    Ui::SetPinDialog *ui;
    QStandardItemModel* peripheralItemModel;
    QStandardItemModel* pinItemModel;
    QList<QString> ArduinoPinMap;
    QList<QString> IOMap;
    QList<QString> ArduinoPinInfo;
    bool itemReady;
    //Return PeripheralCache
    QList<Peripheral> PeripheralCache;
    QList<int> pinSelected;
    QList<int> pinCurrentSelected;
    int currentRow;
    Device newDev;
    int devType;
    int devID;
    int maxNum;
    int index;
};

#endif // SETPINDIALOG_H
