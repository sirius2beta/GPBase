#include "dnvideomanager.h"

DNVideoManager::DNVideoManager(QObject *parent)
    : QObject{parent}
{

}

void DNVideoManager::init()
{

}

void DNVideoManager::addVideoItem(QString title, int boatID, int videoNo, int formatNo, int PCPort)
{
    VideoItem* newvideoitem = new VideoItem(this, title, boatID, videoNo, formatNo, PCPort);
    videoList.append(newvideoitem);
}
