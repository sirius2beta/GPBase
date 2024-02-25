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

void VideoWindow::updateFormat()
{
    if(ui->videoportComboBox->currentIndex() == -1){
        if(ui->videoportComboBox->model()->rowCount()>0){

            ui->videoportComboBox->setCurrentIndex(0);
        }
    }
    if(ui->videoFormatcomboBox->currentIndex() == -1){
        if(ui->videoFormatcomboBox->model()->rowCount()>0){

            ui->videoFormatcomboBox->setCurrentIndex(0);
        }
    }
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
    int port = _videoItem->PCPort();
    if(proxyMode){
        port += 100;
    }

    _videoItem->play(encoder, proxyMode);

    ui->playButton->setEnabled(false);
    //int boatID = _core->boatManager()->getBoatbyIndex(ui->boatcomboBox->currentIndex())->ID();
    //QHostAddress ip = QHostAddress(_core->boatManager()->getBoatbyIndex(ui->boatcomboBox->currentIndex())->currentIP());
    //QString msg = ui->videoportComboBox->currentText()+" "+ui->videoFormatcomboBox->currentText()+" "+encoder+" nan"+" 90"+" "+QString::number(port);

    //emit sendMsg(ip, char(COMMAND), msg.toLocal8Bit());

    QTimer::singleShot(100,[=]{
        ui->playButton->setEnabled(true);
    });
    isPlaying = true;
    qDebug()<<"play";


}

void VideoWindow::onStop()
{
    std::string s = QString(ui->videoportComboBox->currentText()).toStdString();
    QHostAddress ip = QHostAddress(_core->boatManager()->getBoatbyIndex(ui->boatcomboBox->currentIndex())->currentIP());
    emit sendMsg(ip, char(QUIT), QByteArray(s.c_str()));
    isPlaying = false;
    _videoItem->stop();
}


void VideoWindow::setVideoFormat(int ID, QStringList videoformat)
{   
    /*
    int index = _core->boatManager()->getIndexbyID(ID);
    if(ui->boatcomboBox->currentIndex() == index){


        int preVideoNo = ui->videoportComboBox->currentIndex();
        //ui->videoportComboBox->clear();
        //ui->videoFormatcomboBox->clear();

        QStringList videoFormatList;
        QString thisvideoNo; //videox
        int index = -1;
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
        ui->videoportComboBox->setItemData(index, videoFormatList);
        //qDebug()<<"VideoWindow "<<_videoItem->index()<<", pre-index count: "<< ui->videoportComboBox->count()<<" , videoNo: "<<videoNo;
        videoFormatList = ui->videoportComboBox->currentData().toStringList();
        for(int i = 0;i<videoFormatList.size(); i++){
            ui->videoFormatcomboBox->addItem(videoFormatList[i],0);

        }
        if(ui->videoportComboBox->count() > preVideoNo){
            if(preVideoNo == -1){
                ui->videoportComboBox->setCurrentIndex(videoNo);
            }else{
                ui->videoportComboBox->setCurrentIndex(preVideoNo);
                setVideoNo(preVideoNo);
            }

        }

        if(ui->videoFormatcomboBox->count() > formatNo){
            ui->videoFormatcomboBox->setCurrentIndex(formatNo);
        }

    }
    */

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
    //worker->setGstcmd(gstcmd);


    QDockWidget* dockwidget = (QDockWidget*)parent();
    dockwidget->setWindowTitle(_videoItem->title());
    //ui->screen_text->setText(QString("%1\n%3\n(port : %2)").arg(title, QString::number(port),ui->videoportComboBox->currentText()));

}



void VideoWindow::handleResult(const QPixmap &result)
{
   ;
   ui->screen_text->setPixmap(result.scaledToHeight(ui->screen_text->height()));

}

void VideoWindow::clearScreen(){
    ui->screen_text->clear();
    ui->screen_text->setText(QString("%1\n%3\n(port : %2)").arg(_videoItem->title(), QString::number(_videoItem->PCPort()),ui->videoportComboBox->currentText()));
}



void VideoWindow::setVideoNo(int i)
{
    _videoItem->setVideoNo(ui->videoportComboBox->itemText(i).split("o")[1].toInt());
    videoNo = i;
    //ui->videoFormatcomboBox->clear();
    QStringList formatList = ui->videoportComboBox->itemData(i).toStringList();
    for(int j = 0; j<formatList.size(); j++){
        ui->videoFormatcomboBox->addItem(formatList[j],0);
    }
    setFormat();

}

void VideoWindow::setFormatNo(int i)
{
    _videoItem->setFormatNo(i);
    formatNo = i;

}

void VideoWindow::onSetFormatNo(int i)
{
    _videoItem->setFormatNo(i);
    formatNo = i;
    settings->setValue(QString("%1/w%2/formatno").arg(_config,QString::number(_videoItem->index())), i);
}

void VideoWindow::onSetBoatNo(int i)
{
    if(ui->boatcomboBox->count()!=0){
        _videoItem->setBoatID(_core->boatManager()->getIDbyInex(i));
    }
    qDebug()<<"set boatNO";
    //ui->videoFormatcomboBox->clear();
    //ui->videoportComboBox->clear();

}

void VideoWindow::onConnectionChanged()
{
    if(isPlaying){
        onPlay();
        qDebug()<<"VideoWindow::onConnectionChanged";
    }

}
