#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include <QObject>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QWidget>
#include <QStandardItemModel>


class GPBCore;
class VideoItem : public QObject
{
    Q_OBJECT
public:
    explicit VideoItem(QObject *parent = nullptr, GPBCore* core = nullptr, int index=-1, QString title=QString(), int boatID=-1, int videoNo=-1, int formatNo=-1, int PCPort=0);
    ~VideoItem();
    void setDisplay(WId xwinid);
    void play();
    void play(QString encoder, bool proxy);
    void stop();

    void setTitle(QString title);
    void setPCPort(int port);
    void setBoatID(int ID);
    void setIndex(int index);
    void setVideoNo(int index);
    void setFormatNo(int no);
    void setVideoFormat(QStringList videoformat);
    void setWID(WId wid){_xwinid = wid;}
    void setConnectionPriority(int connectionType);
    void setVideoInfo(bool i) { _isVideoInfo = i; }

    QString title() { return _title;   }
    int boatID() {  return _boatID; }
    int PCPort() {  return _PCPort; }
    int port() {return _proxy?(_PCPort+100):_PCPort;}
    int index() {return _index; }
    int videoNo() { return _videoNo;}
    bool isPlaying(){ return _isPlaying;}
    int connectionPriority() { return _connectionPriority;}
    bool videoInfo() { return _isVideoInfo; }

    QString encoder() {return _encoder;}
    QString videoFormat();
    QAbstractItemModel* videoNoModel(){ return _videoNoModel;   }
    QAbstractItemModel* qualityModel(){ return _qualityModel;   }

signals:
    void requestFormat(VideoItem* v); //set _requestFormat = true before sending
    void PCPortChanged(int port);
    void titleChanged(QString title);
    void indexChanged(int index);
    void videoPlayed(VideoItem* v);
    void videoStoped(VideoItem* v);

private:
    GPBCore* _core;

    QString _title;
    int _boatID;
    int _index;
    int _videoNo;
    int _formatNo;
    int _PCPort;
    int _connectionPriority;
    WId _xwinid;
    QString _encoder;
    bool _proxy;
    bool _requestFormat;

    GstElement *_pipeline;
    GstElement *_sink;
    bool _isPlaying;
    bool _isVideoInfo;

    bool _proxyMode;
    QStringList _videoFormatList;
    QStandardItemModel* _videoNoModel;
    QStandardItemModel* _qualityModel;

};

#endif // VIDEOITEM_H
