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
//Q_DECLARE_METATYPE(cv::Mat)

namespace Ui {
class VideoWindow;
}



class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    Worker(QString name , QObject *parent = nullptr);
    ~Worker();

    void setGstcmd(const QString cmd);

signals:
    void resultReady(const QPixmap& result); //工作完成信号

public slots:

    void doWork(const QString cmd=QString()); //工作函数
    void update();
    void exit();
    void stop();
    void restart(const QString cmd);
private:
    //cv::VideoCapture *capture;
    //YOLOV5 *yolov5;
    QString name;
    bool initiated;
    QTimer* timer;
    QString gstcmd;
};



class VideoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWindow(QWidget *parent = nullptr, QString config=QString());
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
    void setBoatList(BoatManager* boatlist);
signals:
    void sendCommand(int ID, QString command, int PCPort);
    void sendMsg(QHostAddress addr, char topic, QByteArray command);

    void nextframe();
    void order(const QString cmd=QString());
    void stopworker();

public slots:
    void handleResult(const QPixmap& result);
    void setVideoFormat(int ID, QStringList videoformat);
    void resetBoatName(QString boatname,QString newboatname);
    void AddBoat(Boat *boat);
    void onBoatNameChange(int boatIndex, QString newname);
    void onDeleteBoat(QString boatname);
    void onConnectionChanged();

protected slots:
    void onSettings();
    void changeSettings(QString _title, QString boatIP, int boatPort, int videono, int formatno, bool video_info);
    void onPlay();
    void onStop();
    void onUDPMsg();
    void onSetFormatNo(int i);
    void onSetBoatNo(int i);


private:
    Ui::VideoWindow *ui;
    int PCPort;

    int index;
    int index2;
    QString title;
    //QString boatName;
    QSettings* settings;
    int formatNo;
    int videoNo;
    bool isVideoInfo;

    GstElement *pipeline;
    GstElement *sink;
    bool isPlaying;

    QString _config;
    QThread workThread; //worker 线程
    Worker *worker;
    QUdpSocket *serverSocket;
    QUdpSocket *clientSocket;
    BoatManager* boatList;


};

#endif // VIDEOWINDOW_H

