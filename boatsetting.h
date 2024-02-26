﻿
#ifndef BOATSETTING_H
#define BOATSETTING_H

#include <QWidget>
#include <QStandardItemModel>
#include <QDebug>
#include <QMap>
#include <QSettings>
#include <QMessageBox>
#include "boatmanager.h"
#include "adddevicedialog.h"
#include "qhostaddress.h"
#include "boatitem.h"

class GPBCore;

namespace Ui {
class BoatSetting;
}

class BoatSetting : public QWidget
{
    Q_OBJECT

public:
    explicit BoatSetting(QWidget *parent = nullptr);
    ~BoatSetting();
    void init(GPBCore* core);
    void addDevice(int ID, QString Devicename, char format, bool IO);
    void deleteDevice(int ID);
    void addBoard(int ID, QString boardName, QString boardType, bool connected);
    void upDateDeviceTableView();

    enum Connection {Auto = 0, Primary = 1, Secondary=2};

signals:
    void connectionTypeChanged(int connectiontype);

public slots:
    void onBoatNameChange();
    void onAddBoat();
    void onDeleteBoat();
    void onBoatSelected(int index);
    void onBoatDoubleClicked(QModelIndex index);
    void onChangePIP();
    void onChangeSIP();
    void onAddDeviceButtonClicked();

private:
    Ui::BoatSetting *ui;
    QStandardItemModel* deviceItemModel;
    QStandardItemModel* peripheralItemModel;

    BoatManager* boatManager;
    bool initialized;

};

#endif // BOATSETTING_H
