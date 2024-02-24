#include "dnvideomanager.h"

DNVideoManager::DNVideoManager(QObject *parent)
    : QObject{parent}
{
    QCoreApplication::setOrganizationName("Ezosirius");
    QCoreApplication::setApplicationName("GPlayer_v1");
    settings = new QSettings;
}

void DNVideoManager::init()
{

}

void DNVideoManager::addVideoItem(QString title, int boatID, int videoNo, int formatNo, int PCPort)
{
    VideoItem* newvideoitem = new VideoItem(this, title, boatID, videoNo, formatNo, PCPort);
    videoList.append(newvideoitem);
}
