#include "videoitem.h"

VideoItem::VideoItem(QObject *parent, int index, QString title, int boatID, int videoNo, int formatNo, int PCPort)
    : QObject{parent},
      _title(title),
      _boatID(boatID),
      _index(index),
    _videoNo(videoNo),
    _formatNo(formatNo),
    _PCPort(PCPort),
    _encoder(QString("h264")),
    _proxy(false),
    _isPlaying(false)
{
    _videoNoModel = new QStandardItemModel;
    _qualityModel = new QStandardItemModel;
}

VideoItem::~VideoItem()
{
    if(_isPlaying == false){

    }else{
        gst_element_set_state (_pipeline, GST_STATE_NULL);
        gst_object_unref (_pipeline);
    }
    delete _videoNoModel;
    delete _qualityModel;
}

void VideoItem::setTitle(QString title)
{
    _title = title;
    emit titleChanged(title);
}

void VideoItem::setPCPort(int port)
{
    _PCPort = port;
    emit PCPortChanged(port);
}

void VideoItem::setBoatID(int ID)
{
    _boatID = ID;
    emit boatIDSet(ID);
}

void VideoItem::setIndex(int index)
{
    _index = index;
    emit indexChanged(index);
}

void VideoItem::setVideoNo(int videoNo)
{
    _qualityModel->removeRows(0, _qualityModel->rowCount());
    for(const auto &formatlist:_videoFormatList){
        if(formatlist[0] == _videoNo){
            QStringList fl = formatlist.split(' ');
            fl.pop_front();
            int current = _videoNoModel->rowCount();
            QStandardItem* item = new QStandardItem(fl.join(" "));
            _videoNoModel->setItem(current, 0, item);
        }
    }
}

void VideoItem::setVideoFormat(QStringList videoformat)
{
    int index = -1;
    QString currentvideoNo = QString();
    for(const auto &vf:videoformat){

        QString videoNo = vf.split(' ')[0];
        if(currentvideoNo != videoNo){
            currentvideoNo = videoNo;
            int current = _videoNoModel->rowCount();
            QStandardItem* item = new QStandardItem(currentvideoNo);
            _videoNoModel->setItem(current, 0, item);

        }
        QStringList vfl = vf.split(' ');
        if(vfl[2].split('=')[1].toInt()<= 1920){  //limit with<1920
            QString vfstring = vfl.join(' ');
            _videoFormatList<<vfstring; //save videoformat of videox to videoFormatList
        }
    }

}

void VideoItem::setDisplay(WId xwinid)
{
    _xwinid = xwinid;
}

void VideoItem::play(QString encoder, bool proxy)
{
    QString gstcmd;
    _encoder = encoder;
    _proxy = proxy;

    if(false){
        if(encoder == "h264"){
            gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtph264depay ! avdec_h264 ! videoconvert  !\
             textoverlay text=\"%2\n%3\nPort:%1\" valignment=top halignment=right font-desc=\"Sans, 14\" !\
             glimagesink name=mySink2").arg(QString::number(_PCPort),_title, QString::number(_videoNo));
        }else{
             gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
             textoverlay text=\"%2\n%3\nPort:%1\" valignment=top halignment=right font-desc=\"Sans, 14\" !\
             glimagesink name=mySink2").arg(QString::number(_PCPort),_title, QString::number(_videoNo));
        }

    }else{
        if(encoder == "h264"){
            gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtph264depay ! avdec_h264 ! videoconvert  !\
             glimagesink name=mySink2").arg(QString::number(_PCPort));
        }else{
             gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
             glimagesink name=mySink2").arg(QString::number(_PCPort));
        }
    }


    if(!_isPlaying){
        _pipeline= gst_parse_launch(gstcmd.toLocal8Bit(), NULL);
        _sink = gst_bin_get_by_name((GstBin*)_pipeline,"mySink2");
        gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (_sink), _xwinid);
        gst_element_set_state (_pipeline,
            GST_STATE_PLAYING);
        _isPlaying = true;
    }
}

void VideoItem::stop()
{
    if(_isPlaying == false){

    }else{
        gst_element_set_state (_pipeline, GST_STATE_NULL);
        _isPlaying = false;
    }
}

QString VideoItem::videoFormat()
{
    return _qualityModel->item(_formatNo, 0)->text();

}
