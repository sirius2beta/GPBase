#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QWidget>
#include <QDockWidget>
#include <QSettings>
#include <QTimer>
#include <QList>


#include <QObject>
#include <QThread>
#include <QMetaType>
#include <QUdpSocket>
#include <QMap>
#include <QHostAddress>
#include "videosettingsdialog.h"
#include "boatmanager.h"
#include "QTypes.h"
#include "videoitem.h"

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
    void init(VideoItem* videoItem);
    void setFormat();

    void setVideoInfo(bool i);
    void clearScreen();
    void setVideoNo(int i);
    void setFormatNo(int i);

signals:
    void sendMsg(QHostAddress addr, char topic, QByteArray command);

public slots:
    void handleResult(const QPixmap& result);
    void setVideoFormat(int index, QStringList videoformat);
    void onConnectionChanged();
    void onPCPortChanged(int port);
    void onTitleChanged(QString t);
    void onIndexChanged(int i);

protected slots:
    void onSettings();
    void changeSettings(VWSetting settings);
    void onPlay();
    void onStop();
    void onSetFormatNo(int i);
    void onSetBoatNo(int i);


private:
    Ui::VideoWindow *ui;
    VideoItem* _videoItem;
    bool proxyMode;

    //int index;
    QSettings* settings;
    int formatNo;
    int videoNo;
    bool isVideoInfo;

    GstElement *pipeline;
    GstElement *sink;
    bool isPlaying;

    QString _config;
    GPBCore* _core;

};

#endif // VIDEOWINDOW_H

