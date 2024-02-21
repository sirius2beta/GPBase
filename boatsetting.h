
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
    void setconfig(QString conf);
    void addDevice(int ID, QString Devicename, char format, bool IO);
    void deleteDevice(int ID);
    void addBoard(int ID, QString boardName, QString boardType, bool connected);
    BoatItem* currentBoat();
    void upDateDeviceTableView();
    enum Connection {Auto = 0, Primary = 1, Secondary=2};
    int connectionType();

signals:
    void changeBoatName(int boatIndex, QString newboatname);
    void deleteBoat(QString boatname);
    void connectionTypeChanged();


public slots:
    void onMsg(QByteArray data);
    void onConnected(int ID, bool isprimary);
    void onDisonnected(int ID, bool isprimary);
    BoatItem* appendBoat(QString boatname, int ID, QString PIP, QString SIP);
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
    BoatManager* boatManager;
    bool initialized;

};

#endif // BOATSETTING_H
