#include "videoitem.h"

VideoItem::VideoItem(QObject *parent, QString title, int boatID, int videoNo, int formatNo, int PCPort)
    : QObject{parent},
    _title(title),
    _boatID(boatID),
    _videoNo(videoNo),
    _formatNo(formatNo),
    _PCPort(PCPort)
{
    _videoNoModel = new QStandardItemModel;
    _qualityModel = new QStandardItemModel;
}

VideoItem::~VideoItem()
{
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

void VideoItem::play()
{

}

