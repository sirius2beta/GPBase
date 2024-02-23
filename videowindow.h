#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QWidget>
#include <QDockWidget>
#include <QSettings>
#include <QTimer>
#include <QList>
//#include <opencv2/opencv.hpp>
#include "videosettingsdialog.h"

//#include "yolov5.h"
#include <QObject>
#include <QThread>
#include <QMetaType>
#include <QUdpSocket>
#include <QMap>
#include <QHostAddress>
#include "boatmanager.h"
#include "QTypes.h"

//Q_DECLARE_METATYPE(cv::Mat)

namespace Ui {
class VideoWindow;
}

class VideoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWindow(QWidget *parent = nullptr, QString config=QString(), GPBCore *core = nullptr);
    ~VideoWindow();
    void init();
    void setFormat();
    void setPCPort(int p);
    void setTitle(QString t);
    void setIndex(int i);
    void setVideoInfo(bool i);
    void clearScreen();
    void setVideoNo(int i);
    void setFormatNo(int i);

signals:
    void sendCommand(int ID, QString command, int PCPort);
    void sendMsg(QHostAddress addr, char topic, QByteArray command);

    void nextframe();
    void order(const QString cmd=QString());
    void stopworker();

public slots:
    void handleResult(const QPixmap& result);
    void setVideoFormat(int ID, QStringList videoformat);
    void onConnectionChanged();

protected slots:
    void onSettings();
    void changeSettings(VWSetting settings);
    void onPlay();
    void onStop();
    void onUDPMsg();
    void onSetFormatNo(int i);
    void onSetBoatNo(int i);


private:
    Ui::VideoWindow *ui;
    int PCPort;
    bool proxyMode;

    int index;
    int index2;
    QString title;
    QSettings* settings;
    int formatNo;
    int videoNo;
    bool isVideoInfo;

    GstElement *pipeline;
    GstElement *sink;
    bool isPlaying;

    QString _config;
    QThread workThread; //worker 线程
    //Worker *worker;
    QUdpSocket *serverSocket;
    QUdpSocket *clientSocket;
    GPBCore* _core;

};

#endif // VIDEOWINDOW_H

