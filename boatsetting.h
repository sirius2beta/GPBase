﻿
#ifndef BOATSETTING_H
#define BOATSETTING_H

#include <QWidget>
#include <QStandardItemModel>
#include <QDebug>
#include <QMap>
#include <QSettings>
#include <QMessageBox>
#include "boats.h"
#include "adddevicedialog.h"


namespace Ui {
class BoatSetting;
}

class BoatSetting : public QWidget
{
    Q_OBJECT

public:
    explicit BoatSetting(QWidget *parent = nullptr);
    ~BoatSetting();
    void initSettings(Boats *_boatList);
    QString getIP(QString boatname);
    bool isPrimary(QString boatname);
    void setconfig(QString conf);
    void addDevice(int ID, QString Devicename, char format, bool IO);
    void deleteDevice(int ID);
    void addBoard(int ID, QString boardName, QString boardType, bool connected);
    _Boat* currentBoat();
    void upDateDeviceTableView();
    enum Connection {Auto = 0, Primary = 1, Secondary=2};
    int connectionType();

signals:
    void changeBoatName(QString boatname, QString newboatname);
    void AddBoat(QString newname, int ID, QString PIP, QString SIP);
    void vAddBoat(QString newname);
    void ChangeIP(QString boatname, QString PIP, QString SIP);
    void deleteBoat(QString boatname);
    void sendMsg(QString boatname, char topic, QByteArray command);
    void connectionTypeChanged();


public slots:
    void onMsg(QByteArray data);
    void onConnected(int ID, bool isprimary);
    void onDisonnected(int ID, bool isprimary);
    void appendBoat(QString boatname, int ID, QString PIP, QString SIP);
    void onBoatNameChange();
    void onAddBoat();
    void onDeleteBoat();
    void onBoatSelected(int index);
    void onBoatDoubleClicked(QModelIndex index);
    void onChangeIP();
    void onAddDeviceButtonClicked();

private:
    Ui::BoatSetting *ui;
    QStandardItemModel* boatItemModel;
    QStandardItemModel* deviceItemModel;
    QStandardItemModel* peripheralItemModel;
    QSettings *settings;
    QString config;
    Boats* boatList;
    bool initialized;

};

#endif // BOATSETTING_H
