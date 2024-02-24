#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include <QObject>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QWidget>
#include <QStandardItemModel>

class VideoItem : public QObject
{
    Q_OBJECT
public:
    explicit VideoItem(QObject *parent = nullptr, QString title=QString(), int boatID=-1, int videoNo=-1, int formatNo=-1, int PCPort=0);
    ~VideoItem();
    void setDisplay(WId xwinid);
    void play(QString encoder);
    void stop();

    void setTitle(QString title);
    void setPCPort(int port);
    void setBoatID(int ID);
    void setIndex(int index);
    void setVideoNo(int videoNo);
    void setVideoFormat(QStringList videoformat);
    void setWID(WId wid){_xwinid = wid;}

    QString title() { return _title;   }
    int boatID() {  return _boatID; }
    int PCPort() {  return _PCPort; }
    int index() {return _index; }
    QAbstractItemModel* videoNoModel(){ return _videoNoModel;   }
    QAbstractItemModel* qualityModel(){ return _qualityModel;   }

signals:
    void boatIDSet(int ID);
    void PCPortChanged(int port);
    void titleChanged(QString title);
    void indexChanged(int index);
    void videoPlayed(VideoItem* v);
    void videoStoped(VideoItem* v);
private:
    QString _title;
    int _boatID;
    int _index;
    int _videoNo;
    int _formatNo;
    int _PCPort;
    WId _xwinid;

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
