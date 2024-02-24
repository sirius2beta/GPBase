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

void VideoItem::setBoatID(int ID)
{
    _boatID = ID;
    emit boatIDSet(ID);
}

void VideoItem::setVideoFormat(QStringList videoformat)
{
    QStringList videoFormatList;
    int index = -1;
    /*
    for(const auto &vf:videoformat){
        QString videoNo = vf.split(' ')[0];
        if(videoNo == thisvideoNo){ //the same videox
            QStringList vfl = vf.split(' ');
            vfl.pop_front();
            if(vfl[1].split('=')[1].toInt()<= 1920){  //limit with<1920
                QString vfstring = vfl.join(' ');
                videoFormatList<<vfstring; //save videoformat of videox to videoFormatList
            }

        }else{ //next video port(ex. video1 to video2)
            ui->videoportComboBox->setItemData(index, videoFormatList);
            ui->videoportComboBox->addItem(videoNo, 0);
            thisvideoNo = videoNo;
            videoFormatList.clear();
            index++;
        }
    }
*/
}

void VideoItem::setDisplay(WId xwinid)
{
    _xwinid = xwinid;
}

void VideoItem::play()
{

}

