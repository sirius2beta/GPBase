#include "videowindow.h"
#include "ui_videowindow.h"

#define HEARTBEAT 0x10
#define FORMAT 0x20
#define COMMAND 0x30
#define QUIT 0x40

#include "gpbcore.h"

VideoWindow::VideoWindow(QWidget *parent, QString config, GPBCore* core) :
    QWidget(parent),
    ui(new Ui::VideoWindow),
    proxyMode(false),
    isPlaying(false),
    _config(config)
{
    _core = core;

    ui->setupUi(this);
    ui->playButton->setIcon(QIcon(":/icon/playbutton.png"));
    ui->stopButton->setIcon(QIcon(":/icon/stopbutton.png"));
    ui->settingsButton->setIcon(QIcon(":/icon/settingbutton-02.png"));
    ui->boatcomboBox->setModel(_core->boatManager()->model());



    QCoreApplication::setOrganizationName("Ezosirius");
    QCoreApplication::setApplicationName("GPlayer_v1");
    settings = new QSettings;

    connect(ui->settingsButton,&QPushButton::clicked,this,&VideoWindow::onSettings);
    connect(ui->playButton, &QPushButton::clicked, this, &VideoWindow::onPlay);
    connect(ui->stopButton, &QPushButton::clicked, this, &VideoWindow::onStop);


    connect(ui->videoportComboBox, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, this,&VideoWindow::setVideoNo);
    connect(ui->videoFormatcomboBox, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, this,&VideoWindow::onSetFormatNo);
    connect(ui->boatcomboBox, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, this,&VideoWindow::onSetBoatNo);

}

VideoWindow::~VideoWindow()
{
    delete ui;

}

void VideoWindow::init(VideoItem *videoItem)
{
    _videoItem = videoItem;
    _videoItem->setWID(ui->screen_text->winId());
    ui->videoportComboBox->setModel(videoItem->videoNoModel());
    ui->videoFormatcomboBox->setModel(videoItem->qualityModel());
    connect(videoItem, &VideoItem::titleChanged, this, &VideoWindow::onTitleChanged);
    connect(videoItem, &VideoItem::PCPortChanged, this, &VideoWindow::onPCPortChanged);
    connect(videoItem, &VideoItem::indexChanged, this, &VideoWindow::onIndexChanged);

    setFormat();
}

void VideoWindow::setFormat()
{
    ui->screen_text->setText(QString("%1\n%3\n(port : %2)").arg(_videoItem->title(), QString::number(_videoItem->PCPort()),ui->videoportComboBox->currentText()));
}

void VideoWindow::onPCPortChanged(int port)
{
    ui->screen_text->setText(QString("%1 (port : %2)").arg(_videoItem->title(), QString::number(port)));
}

void VideoWindow::onTitleChanged(QString t)
{
    ui->screen_text->setText(QString("%1 (port : %2)").arg(t, QString::number(_videoItem->PCPort())));
}

void VideoWindow::onIndexChanged(int i)
{
    //index = i;
}

void VideoWindow::setVideoInfo(bool i)
{
    isVideoInfo = i;
}

void VideoWindow::onSettings()
{
    VideoSettingsDialog* dialog = new VideoSettingsDialog(this);
    VWSetting vwsetting;
    vwsetting.title = _videoItem->title();
    vwsetting.videono =videoNo;
    vwsetting.formatno =formatNo;
    vwsetting.video_info = isVideoInfo;
    vwsetting.proxy = proxyMode;
    dialog->setInfo(vwsetting);
    dialog->setFormat(ui->videoFormatcomboBox->model(), ui->videoFormatcomboBox->currentIndex());
    dialog->setVideo(ui->videoportComboBox->model(),ui->videoportComboBox->currentIndex());
    connect(dialog, &VideoSettingsDialog::comit, this, &VideoWindow::changeSettings);
    connect(dialog, &VideoSettingsDialog::comit, this, &VideoWindow::onStop);
    dialog->exec();
}

void VideoWindow::onPlay()
{
    QString encoder = "jpegenc";
    if(ui->H264checkBox->isChecked()){
        encoder = "h264";
    }

    _videoItem->play(encoder, proxyMode);
    ui->playButton->setEnabled(false);
    QTimer::singleShot(100,[=]{
        ui->playButton->setEnabled(true);
    });

    isPlaying = true;
}

void VideoWindow::onStop()
{
    isPlaying = false;
    _videoItem->stop();
}

void VideoWindow::changeSettings(VWSetting settings)
{
    qDebug()<<"change settings";
    _videoItem->setTitle(settings.title);
    setVideoInfo(settings.video_info);
    ui->videoportComboBox->setCurrentIndex(settings.videono);
    ui->videoFormatcomboBox->setCurrentIndex(settings.formatno);
    proxyMode = settings.proxy;

    this->settings->setValue(QString("%1/w%2/videono").arg(_config,QString::number(_videoItem->index())), settings.videono);
    this->settings->setValue(QString("%1/w%2/formatno").arg(_config,QString::number(_videoItem->index())), settings.formatno);
    this->settings->setValue(QString("%1/w%2/title").arg(_config,QString::number(_videoItem->index())), _videoItem->title());
    if(isVideoInfo){
        this->settings->setValue(QString("%1/w%2/videoinfo").arg(_config,QString::number(_videoItem->index())), 1);
    }else{
        this->settings->setValue(QString("%1/w%2/videoinfo").arg(_config,QString::number(_videoItem->index())), 0);
    }
    qDebug()<<"start changesettings3";

    QString gstcmd;
    if(isVideoInfo){
        gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
     textoverlay text=\"%2\n%3\nPort:%1\" valignment=top halignment=right font-desc=\"Sans, 18\" !\
     glimagesink name=mySink2").arg(QString::number(_videoItem->PCPort()),_videoItem->title(),ui->videoportComboBox->currentText());
    }else{
        gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
     glimagesink name=mySink2").arg(QString::number(_videoItem->PCPort()),_videoItem->title(),ui->videoportComboBox->currentText());
    }

    QDockWidget* dockwidget = (QDockWidget*)parent();
    dockwidget->setWindowTitle(_videoItem->title());
    //ui->screen_text->setText(QString("%1\n%3\n(port : %2)").arg(title, QString::number(port),ui->videoportComboBox->currentText()));

}

void VideoWindow::clearScreen(){
    ui->screen_text->clear();
    ui->screen_text->setText(QString("%1\n%3\n(port : %2)").arg(_videoItem->title(), QString::number(_videoItem->PCPort()),ui->videoportComboBox->currentText()));
}

void VideoWindow::setVideoNo(int i)
{
    qDebug()<<"VideoWindow::setVideoNo: "<<i;
    _videoItem->setVideoNo(i);
}

void VideoWindow::onSetFormatNo(int i)
{
    _videoItem->setFormatNo(i);
    formatNo = i;
    settings->setValue(QString("%1/w%2/formatno").arg(_config,QString::number(_videoItem->index())), i);
}

void VideoWindow::onSetBoatNo(int i)
{
    qDebug()<<"set boatNO ui "<<ui->boatcomboBox->count();
    if(ui->boatcomboBox->count()!=0){
        _videoItem->setBoatID(_core->boatManager()->getIDbyInex(i));
        qDebug()<<"set boatNO "<<_core->boatManager()->getIDbyInex(i);
    }
}

void VideoWindow::onConnectionChanged()
{
    if(isPlaying){
        onPlay();
        qDebug()<<"VideoWindow::onConnectionChanged";
    }
}
