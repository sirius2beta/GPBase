#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QMainWindow>
#include <QSettings>
#include <QDebug>
#include <QDockWidget>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QUdpSocket>
#include <QThread>
#include <QByteArray>
#include "boatsetting.h"
#include "sensorwidget.h"
#include "networkmanager.h"
#include "boatmanager.h"
#include "heartbeat.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, QString config=QString());
    ~MainWindow();

    void initBoatSettings();
    void initVideoWindows();
    void initSensorWidget();
    QMap<QString,QString> USV1format;
    QMap<QString,QString> USV2format;
signals:
    void updateFormat(QMap<QString,QString> format);

    void changePCPIP(QString ip);
    void changePCSIP(QString ip);

    void connectionChanged();

protected slots:
    void openCreateWindowDialog();
    void addVideoWindow(int index, bool central_widget);
    void setConfig(QString config);
    void onConnected(int ID, bool isprimary);
    void onDisonnected(int ID, bool isprimary);
    void onNewBoat(Boat *newboat);
    void onConnectionTypeChanged();


private:
    Ui::MainWindow *ui;
    QSettings* settings;
    BoatSetting* boatSetting;
    sensorWidget* sensor_widget;
    /*
    QUdpSocket *serverSocket;
    QUdpSocket *clientSocket;
    */
    QString _config;
    HeartBeat* primaryHeartBeat;
    HeartBeat* secondaryHeartBeat;
    QString PCPIP;
    QString BoatPIP;
    QString PCSIP;
    QString BoatSIP;
    BoatManager* boatList;
    NetworkManager* networkManager;


    char HEARTBEAT = 0x10;

};
#endif // MAINWINDOW_H
