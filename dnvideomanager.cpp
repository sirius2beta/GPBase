﻿#include "dnvideomanager.h"
#include "gpbcore.h"
#include "QTypes.h"
DNVideoManager::DNVideoManager(QObject *parent, GPBCore* core)
    : QObject{parent},
      _core(core)
{
    QCoreApplication::setOrganizationName("Ezosirius");
    QCoreApplication::setApplicationName("GPlayer_v1");
    settings = new QSettings;

}

void DNVideoManager::init()
{
    int window_count = 4;
    QString _config = _core->config();
    for(int i = 0; i < window_count; i++){
        //create settings if first time opened
        if(settings->value(QString("%1/w%2/in_port").arg(_config,QString::number(i))) == QVariant()){
            QList<QString> formatlist = {"video0", "YUYV", "640-480-15", "nan", "80", "192.168.0.100", "5200"};
            settings->setValue(QString("%1/w%2/boat_name").arg(_config,QString::number(i)),QString("unknown"));
            settings->setValue(QString("%1/w%2/in_port").arg(_config,QString::number(i)),5200+i);
            settings->setValue(QString("%1/w%2/title").arg(_config,QString::number(i)),QString("window%1").arg(i));
            settings->setValue(QString("%1/w%2/videoinfo").arg(_config,QString::number(i)), 1);
            settings->setValue(QString("%1/w%2/formatno").arg(_config,QString::number(i)), 0);
        }
        QString title = settings->value(QString("%1/w%2/title").arg(_config,QString::number(i))).toString();
        int PCPort = 5201+i;
        if(_core->boatManager()->size() > 0){
            addVideoItem(i, title, _core->boatManager()->getIDbyInex(0), -1, -1, PCPort);
        }else{
            addVideoItem(i, title, -1, -1, -1, PCPort);
        }
    }

}

void DNVideoManager::addVideoItem(int index, QString title, int boatID, int videoNo, int formatNo, int PCPort)
{
    qDebug()<<"----------------"<<boatID;
    VideoItem* newvideoitem = new VideoItem(this, _core, index, title, boatID, videoNo, formatNo, PCPort);
    videoList.append(newvideoitem);
    connect(newvideoitem, &VideoItem::videoPlayed, this, &DNVideoManager::onPlay);
    connect(newvideoitem, &VideoItem::videoStoped, this, &DNVideoManager::onStop);
    connect(newvideoitem, &VideoItem::boatIDSet, this, &DNVideoManager::onBoatIDSet);

}

void DNVideoManager::onPlay(VideoItem* videoItem)
{
    QHostAddress ip = QHostAddress(_core->boatManager()->getBoatbyID(videoItem->boatID())->currentIP());
    QString msg = "video"+QString::number(videoItem->videoNo())+" "+videoItem->videoFormat()+" "+videoItem->encoder()+" nan"+" 90"+" "+QString::number(videoItem->port());
    emit sendMsg(ip, char(COMMAND), msg.toLocal8Bit());
}

void DNVideoManager::onStop(VideoItem* videoItem)
{

}

void DNVideoManager::onBoatAdded()
{
    for(int i = 0; i<videoList.size(); i++){
        if(videoList[i]->boatID() == -1){
            videoList[i]->setVideoNo(0);
        }
    }
}

void DNVideoManager::onBoatIDSet(VideoItem* videoItem)
{
    QHostAddress addr(_core->boatManager()->getBoatbyID(videoItem->boatID())->currentIP());
    emit sendMsg(addr,char(FORMAT),"qformat");
    qDebug()<<"format call";
}

void DNVideoManager::setVideoFormat(int ID, QStringList videoformat)
{
    for(int i = 0; i < videoList.size(); i++){
        qDebug()<<"set format, vid:"<<videoList[i]->boatID()<<", bid"<<ID;
        if(videoList[i]->boatID() == ID){
            qDebug()<<"set format, index:"<<_core->boatManager()->getIndexbyID(ID);
            videoList[i]->setVideoFormat(videoformat);

        }
    }
}
