#ifndef DNVIDEOMANAGER_H
#define DNVIDEOMANAGER_H

#include <QObject>
#include "videoitem.h"

class DNVideoManager : public QObject
{
    Q_OBJECT
public:
    explicit DNVideoManager(QObject *parent = nullptr);
    void init();
    void addVideoItem(QString title, int boatID, int videoNo, int formatNo, int PCPort);

signals:
private:
    QVector<VideoItem*> videoList;
};

#endif // DNVIDEOMANAGER_H
