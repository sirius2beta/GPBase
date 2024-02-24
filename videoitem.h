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
    void play();

    void setBoatID(int ID);
    void setVideoFormat(QStringList videoformat);

    int boatID() {  return _boatID; }
    QAbstractItemModel* videoNoModel(){ return _videoNoModel;   }
    QAbstractItemModel* qualityModel(){ return _qualityModel;   }

signals:
    void boatIDSet(int ID);
private:
    QString _title;
    int _boatID;
    int _videoNo;
    int _formatNo;
    int _PCPort;
    WId _xwinid;

    bool _isPlaying;
    bool _isVideoInfo;

    bool _proxyMode;

    QStandardItemModel* _videoNoModel;
    QStandardItemModel* _qualityModel;

};

#endif // VIDEOITEM_H
