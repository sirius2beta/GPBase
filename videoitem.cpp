#include "videoitem.h"
#include "gpbcore.h"
#include <QDebug>

#define HEARTBEAT 0x10
#define FORMAT 0x20
#define COMMAND 0x30
#define QUIT 0x40

VideoItem::VideoItem(QObject *parent, GPBCore* core, int index, QString title, int boatID, int videoNo, int formatNo, int PCPort)
    : QObject{parent},
      _core(core),
      _title(title),
      _boatID(boatID),
      _index(index),
    _videoNo(videoNo),
    _formatNo(formatNo),
    _PCPort(PCPort),
    _encoder(QString("h264")),
    _proxy(false),
    _requestFormat(true),
    _isPlaying(false),
    _connectionPriority(0)
{
    _videoNoModel = new QStandardItemModel;
    _qualityModel = new QStandardItemModel;
    QStandardItem* dummyitem1 = new QStandardItem("fl");
    QStandardItem* dummyitem2 = new QStandardItem("fl");

    //set and remove item so QCombobox can keep index updated
    _videoNoModel->setItem(0,0,dummyitem1);
    _videoNoModel->removeRows(0,1);
    _qualityModel->setItem(0,0,dummyitem2);
    _qualityModel->removeRows(0,1);


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
    if(_boatID != ID){
        _videoNoModel->removeRows(0,_videoNoModel->rowCount());
        _boatID = ID;
    }

    _qualityModel->removeRows(0,_qualityModel->rowCount());
    _requestFormat = true;
    emit requestFormat(this);
}

void VideoItem::setIndex(int index)
{
    _index = index;
    emit indexChanged(index);
}

void VideoItem::setVideoNo(int index)
{
    _qualityModel->removeRows(0,_qualityModel->rowCount());
    int currentNo = -1;
    int currentindex = -1;
    for(const auto &formatlist:_videoFormatList){
        //qDebug()<<"setvideoNO:"<<formatlist.split(' ')[0].split('o')[1]<<","<<videoNo;
        if(formatlist.split(' ')[0].split('o')[1].toInt() != currentNo){
            currentNo = formatlist.split(' ')[0].split('o')[1].toInt();
            currentindex ++;
            _videoNo = currentNo;
        }
        if(currentindex == index){
            QStringList fl = formatlist.split(' ');
            fl.pop_front();
            int current = _qualityModel->rowCount();
            QStandardItem* item = new QStandardItem(fl.join(" "));
            _qualityModel->setItem(current, 0, item);
        }
    }
    emit UIUpdateFormat();
}

void VideoItem::setVideoFormat(QStringList videoformat)
{
    if(!_requestFormat) return;
    _requestFormat = false;
    int index = -1;
    QStandardItem* item = new QStandardItem("fl");
    QString currentvideoNo = QString();
    for(const auto &vf:videoformat){

        QString videoNo = vf.split(' ')[0];
        if(currentvideoNo != videoNo){
            currentvideoNo = videoNo;
            int current = _videoNoModel->rowCount();

                QStandardItem* item = new QStandardItem(currentvideoNo);
                _videoNoModel->setItem(current, 0, item);




            qDebug()<<"DNVideoManager::setVideoFormat:"<<currentvideoNo;

        }
        QStringList vfl = vf.split(' ');
        if(vfl[2].split('=')[1].toInt()<= 1920){  //limit with<1920
            QString vfstring = vfl.join(' ');
            _videoFormatList<<vfstring; //save videoformat of videox to videoFormatList
        }
    }
    if(_videoNoModel->rowCount() >0){
        setVideoNo(0);
    }
    emit UIUpdateFormat();

}

void VideoItem::setFormatNo(int no)
{
    _formatNo = no;
}

void VideoItem::setDisplay(WId xwinid)
{
    _xwinid = xwinid;
}

void VideoItem::setConnectionPriority(int connectionType)
{
    qDebug()<<"VideoItem:: connectionTypeChanged: pre:"<<_connectionPriority<<", now:"<<connectionType;
    if(_connectionPriority != connectionType){
        _connectionPriority = connectionType;
        if(_isPlaying){
            stop();
            play();
        }else{
            play();
        }
    }

}

void VideoItem::play()
{
    if(_videoNo == -1 || _formatNo == -1) return;

    QString gstcmd;
    if(false){
        if(_encoder == "h264"){
            gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtph264depay ! avdec_h264 ! videoconvert  !\
             textoverlay text=\"%2\n%3\nPort:%1\" valignment=top halignment=right font-desc=\"Sans, 14\" !\
             glimagesink name=mySink2").arg(QString::number(_PCPort),_title, QString::number(_videoNo));
        }else{
             gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
             textoverlay text=\"%2\n%3\nPort:%1\" valignment=top halignment=right font-desc=\"Sans, 14\" !\
             glimagesink name=mySink2").arg(QString::number(_PCPort),_title, QString::number(_videoNo));
        }

    }else{
        if(_encoder == "h264"){
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
    emit videoPlayed(this);
}

void VideoItem::play(QString encoder, bool proxy)
{
    _encoder = encoder;
    _proxy = proxy;
    play();
}

void VideoItem::stop()
{
    if(_isPlaying == false){

    }else{
        gst_element_set_state (_pipeline, GST_STATE_NULL);
        _isPlaying = false;
        emit videoStoped(this);
    }
}

QString VideoItem::videoFormat()
{
    if(_formatNo == -1){
        qDebug()<<"**Warning: VideoItem::videoFormat: _formatNo = -1";
        return QString("");
    }
    return _qualityModel->item(_formatNo, 0)->text();

}
