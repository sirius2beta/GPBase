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
#include "gpbcore.h"
#include "videowindow.h"
#include "videoitem.h"


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


protected slots:
    void openCreateWindowDialog();
    VideoWindow *addVideoWindow(VideoItem* videoItem);
    void setConfig(QString config);




private:

    GPBCore* gpbcore;
    Ui::MainWindow *ui;
    QSettings* settings;


    QString _config;
    QString PCPIP;
    QString BoatPIP;
    QString PCSIP;
    QString BoatSIP;


    //NetworkManager* networkManager;
    SensorWidget* sensor_widget;
    HeartBeat* primaryHeartBeat;
    HeartBeat* secondaryHeartBeat;

};
#endif // MAINWINDOW_H
