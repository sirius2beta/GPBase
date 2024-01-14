
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
#include "boat.h"


namespace Ui {
class BoatSetting;
}

class BoatSetting : public QWidget
{
    Q_OBJECT

public:
    explicit BoatSetting(QWidget *parent = nullptr);
    ~BoatSetting();
    void initSettings(BoatManager *_boatList);
    QString getIP(QString boatname);
    bool isPrimary(int ID);
    void setconfig(QString conf);
    void addDevice(int ID, QString Devicename, char format, bool IO);
    void deleteDevice(int ID);
    void addBoard(int ID, QString boardName, QString boardType, bool connected);
    Boat* currentBoat();
    void upDateDeviceTableView();
    enum Connection {Auto = 0, Primary = 1, Secondary=2};
    int connectionType();

signals:
    void changeBoatName(int boatID, QString newboatname);
    void AddBoat(Boat* boat);
    void vAddBoat(QString newname);
    void ChangeIP(QString boatname, QString PIP, QString SIP);
    void deleteBoat(QString boatname);
    void sendMsg(QHostAddress addr, char topic, QByteArray command);
    void connectionTypeChanged();


public slots:
    void onMsg(QByteArray data);
    void onConnected(int ID, bool isprimary);
    void onDisonnected(int ID, bool isprimary);
    Boat* appendBoat(QString boatname, int ID, QString PIP, QString SIP);
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
    BoatManager* boatList;
    bool initialized;

};

#endif // BOATSETTING_H
