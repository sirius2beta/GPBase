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
    PCPort(0),
    proxyMode(false),
    title(QString("VideoWindow")),
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
    emit stopworker();
    if(isPlaying == false){

    }else{
        gst_element_set_state (pipeline, GST_STATE_NULL);
        gst_object_unref (pipeline);
    }

    delete ui;

}

void VideoWindow::init()
{
    /*
    QHostAddress addr = QHostAddress(boatIP);
    QString cmd = QString("qformat 1");
    serverSocket->writeDatagram(cmd.toUtf8().data(),cmd.toUtf8().size(), addr, 50007);
    qDebug()<<"format "+boatIP;
    */
}

void VideoWindow::setFormat()
{
    ui->screen_text->setText(QString("%1\n%3\n(port : %2)").arg(title, QString::number(PCPort),ui->videoportComboBox->currentText()));
}

void VideoWindow::setPCPort(int p)
{
    PCPort = p;
    ui->screen_text->setText(QString("%1 (port : %2)").arg(title, QString::number(PCPort)));
}


void VideoWindow::setTitle(QString t)
{
    title = t;
    ui->screen_text->setText(QString("%1 (port : %2)").arg(title, QString::number(PCPort)));
}

void VideoWindow::setIndex(int i)
{
    index = i;
}

void VideoWindow::setVideoInfo(bool i)
{
    isVideoInfo = i;
}

void VideoWindow::onSettings()
{
    VideoSettingsDialog* dialog = new VideoSettingsDialog(this);
    VWSetting vwsetting;
    vwsetting.title = title;
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

    QString gstcmd;
    QString encoder = "jpegenc";
    int port = PCPort;
    if(proxyMode){
        port += 100;
    }
    if(isVideoInfo){
        if(ui->H264checkBox->isChecked()){
            gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtph264depay ! avdec_h264 ! videoconvert  !\
             textoverlay text=\"%2\n%3\nPort:%1\" valignment=top halignment=right font-desc=\"Sans, 14\" !\
             glimagesink name=mySink2").arg(QString::number(PCPort),title,ui->videoportComboBox->currentText());
            encoder = "h264";
        }else{
             gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
             textoverlay text=\"%2\n%3\nPort:%1\" valignment=top halignment=right font-desc=\"Sans, 14\" !\
             glimagesink name=mySink2").arg(QString::number(PCPort),title,ui->videoportComboBox->currentText());
        }

    }else{
        if(ui->H264checkBox->isChecked()){
            gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtph264depay ! avdec_h264 ! videoconvert  !\
             glimagesink name=mySink2").arg(QString::number(PCPort));
            encoder = "h264";
        }else{
             gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
             glimagesink name=mySink2").arg(QString::number(PCPort));
        }
    }



    ui->playButton->setEnabled(false);
    int boatID = _core->boatManager()->getBoatbyIndex(ui->boatcomboBox->currentIndex())->ID();
    QHostAddress ip = QHostAddress(_core->boatManager()->getBoatbyIndex(ui->boatcomboBox->currentIndex())->currentIP());
    QString msg = ui->videoportComboBox->currentText()+" "+ui->videoFormatcomboBox->currentText()+" "+encoder+" nan"+" 90"+" "+QString::number(port);
    emit sendMsg(ip, char(COMMAND), msg.toLocal8Bit());

    ui->playButton->setEnabled(false);
    QTimer::singleShot(100,[=]{
        ui->playButton->setEnabled(true);
    });


    qDebug()<<"play";
    if(!isPlaying){
        pipeline= gst_parse_launch(gstcmd.toLocal8Bit(), NULL);
        sink = gst_bin_get_by_name((GstBin*)pipeline,"mySink2");
        WId xwinid = ui->screen_text->winId();
        gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (sink), xwinid);
        gst_element_set_state (pipeline,
            GST_STATE_PLAYING);
        isPlaying = true;
    }

}

void VideoWindow::onStop()
{
    std::string s = QString(ui->videoportComboBox->currentText()).toStdString();
    QHostAddress ip = QHostAddress(_core->boatManager()->getBoatbyIndex(ui->boatcomboBox->currentIndex())->currentIP());
    emit sendMsg(ip, char(QUIT), QByteArray(s.c_str()));

    if(isPlaying == false){

    }else{
        gst_element_set_state (pipeline, GST_STATE_NULL);
        isPlaying = false;
    }
}


void VideoWindow::setVideoFormat(int ID, QStringList videoformat)
{   

    if(ui->boatcomboBox->currentText() == _core->boatManager()->getBoatbyID(ID)->name()){


        int preVideoNo = ui->videoportComboBox->currentIndex();
        ui->videoportComboBox->clear();
        ui->videoFormatcomboBox->clear();

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
        qDebug()<<"VideoWindow "<<this->index<<", pre-index count: "<< ui->videoportComboBox->count()<<" , videoNo: "<<videoNo;
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

}


void VideoWindow::changeSettings(VWSetting settings)
{

    qDebug()<<"change settings";
    setTitle(settings.title);
    setVideoInfo(settings.video_info);
    ui->videoportComboBox->setCurrentIndex(settings.videono);
    ui->videoFormatcomboBox->setCurrentIndex(settings.formatno);
    proxyMode = settings.proxy;

    this->settings->setValue(QString("%1/w%2/videono").arg(_config,QString::number(index)), settings.videono);
    this->settings->setValue(QString("%1/w%2/formatno").arg(_config,QString::number(index)), settings.formatno);
    this->settings->setValue(QString("%1/w%2/title").arg(_config,QString::number(index)), title);
    if(isVideoInfo){
        this->settings->setValue(QString("%1/w%2/videoinfo").arg(_config,QString::number(index)), 1);
    }else{
        this->settings->setValue(QString("%1/w%2/videoinfo").arg(_config,QString::number(index)), 0);
    }
    qDebug()<<"start changesettings3";

    QString gstcmd;
    if(isVideoInfo){
        gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
     textoverlay text=\"%2\n%3\nPort:%1\" valignment=top halignment=right font-desc=\"Sans, 18\" !\
     glimagesink name=mySink2").arg(QString::number(PCPort),title,ui->videoportComboBox->currentText());
    }else{
        gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
     glimagesink name=mySink2").arg(QString::number(PCPort),title,ui->videoportComboBox->currentText());
    }
    //worker->setGstcmd(gstcmd);


    QDockWidget* dockwidget = (QDockWidget*)parent();
    dockwidget->setWindowTitle(title);
    //ui->screen_text->setText(QString("%1\n%3\n(port : %2)").arg(title, QString::number(port),ui->videoportComboBox->currentText()));

}



void VideoWindow::handleResult(const QPixmap &result)
{
   ;
   ui->screen_text->setPixmap(result.scaledToHeight(ui->screen_text->height()));

}

void VideoWindow::clearScreen(){
    ui->screen_text->clear();
    ui->screen_text->setText(QString("%1\n%3\n(port : %2)").arg(title, QString::number(PCPort),ui->videoportComboBox->currentText()));
}



void VideoWindow::setVideoNo(int i)
{
    videoNo = i;
    ui->videoFormatcomboBox->clear();
    qDebug()<<"VideoWindow "<<this->index<<" set videoNo : "<<videoNo;
    QStringList formatList = ui->videoportComboBox->itemData(i).toStringList();
    for(int j = 0; j<formatList.size(); j++){
        ui->videoFormatcomboBox->addItem(formatList[j],0);
    }
    setFormat();

}

void VideoWindow::setFormatNo(int i)
{
    formatNo = i;

}

void VideoWindow::onSetFormatNo(int i)
{
    formatNo = i;
    settings->setValue(QString("%1/w%2/formatno").arg(_config,QString::number(this->index)), i);
    qDebug()<<"setformatno 2: "<<i;
}

void VideoWindow::onSetBoatNo(int i)
{
    if(ui->boatcomboBox->count()!=0){
        qDebug()<<"VideoWindow:request format. IP:"<<_core->boatManager()->getBoatbyIndex(i)->currentIP();
        QHostAddress addr(_core->boatManager()->getBoatbyIndex(i)->currentIP());
        emit sendMsg(addr,char(FORMAT),"qformat");
    }

    ui->videoFormatcomboBox->clear();
    ui->videoportComboBox->clear();

}

void VideoWindow::onConnectionChanged()
{
    if(isPlaying){
        onPlay();
        qDebug()<<"VideoWindow::onConnectionChanged";
    }

}
