#ifndef VIDEOSETTINGSDIALOG_H
#define VIDEOSETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QStringList>
#include <QDebug>


typedef struct
{
    QString title;
    QString boatIP;
    int boatPort;
    int videono;
    int formatno;
    bool video_info;
    bool proxy;
} VWSetting;

Q_DECLARE_METATYPE(VWSetting)

namespace Ui {
class VideoSettingsDialog;
}

class VideoSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VideoSettingsDialog(QWidget *parent = nullptr);
    ~VideoSettingsDialog();
    void setInfo(VWSetting settings);
    void accept();
    QString title();
    void setFormat(QAbstractItemModel *model, int index);
    void setVideo(QAbstractItemModel *model, int index);

signals:
    void comit(VWSetting settings);
protected slots:
    void onTitleChanged(QString title);
    void onOptionsChanged(QString options);
    void onPCPortChanged(QString text);
    void onVideoInfoChanged(int i);
    void onProxyChanged(int i);


private:
    Ui::VideoSettingsDialog *ui;
    QString __boatname;
    bool __video_info;
    QString __PCIP;
    int __PCPort;
    QString __title;
    bool proxy;

    //struct mosquitto *mosq;
};

#endif // VIDEOSETTINGSDIALOG_H
