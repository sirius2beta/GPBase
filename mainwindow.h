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
#include "networksettings.h"
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
    void initNetworkSettings();
    void initSensorWidget();
    QMap<QString,QString> USV1format;
    QMap<QString,QString> USV2format;
signals:
    void updateFormat(QMap<QString,QString> format);
    void AliveResponse(QString ip);
    void setFormat(int ID, QStringList format);
    void changePCPIP(QString ip);
    void changePCSIP(QString ip);
    void sensorMsg(QByteArray data);
    void connectionChanged();

protected slots:
    void openCreateWindowDialog();
    void addVideoWindow(int index, bool central_widget);
    void sendUDPCommand(int ID, QString command, int PCPort);
    void sendMsg(QHostAddress addr, char topic, QByteArray command);
    void onUDPMsg();
    void setConfig(QString config);
    void onPCPIPChanged(QString PCIP);
    void onPCSIPChanged(QString PCIP);
    void onConnected(int ID, bool isprimary);
    void onDisonnected(int ID, bool isprimary);
    void onNewBoat(QString boatname, int ID, QString PIP, QString SIP);
    void onConnectionTypeChanged();


private:
    Ui::MainWindow *ui;
    QSettings* settings;
    NetworkSettings* networkSettings;
    BoatSetting* boatSetting;
    sensorWidget* sensor_widget;
    QUdpSocket *serverSocket;
    QUdpSocket *clientSocket;
    QString _config;
    HeartBeat* primaryHeartBeat;
    HeartBeat* secondaryHeartBeat;
    QString PCPIP;
    QString BoatPIP;
    QString PCSIP;
    QString BoatSIP;
    BoatManager* boatList;


    char HEARTBEAT = 0x10;

};
#endif // MAINWINDOW_H
