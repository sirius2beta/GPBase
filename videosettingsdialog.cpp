#include "videosettingsdialog.h"
#include "ui_videosettingsdialog.h"


VideoSettingsDialog::VideoSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoSettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle("Streaming Settings");

    ui->videonoComboBox->addItem("video0", "video0");
    ui->videonoComboBox->addItem("video1", "video1");
    ui->videonoComboBox->addItem("video2", "video2");
    ui->videonoComboBox->addItem("video3", "video3");
    ui->videonoComboBox->addItem("video4", "video4");
    ui->videonoComboBox->addItem("video5", "video5");
    ui->videonoComboBox->addItem("video6", "video6");

    //ui->formatComboBox->addItem("320 x 240 15fps", "320-240-15");
    ui->formatComboBox->addItem("320 x 240 30fps", "320-240-30");
    //ui->formatComboBox->addItem("640 x 480 15fps", "640-480-15");
    ui->formatComboBox->addItem("640 x 480 30fps", "640-480-30");
    ui->formatComboBox->addItem("800 x 600 20fps", "800-600-20");
    ui->formatComboBox->addItem("800 x 600 30fps", "800-600-30");


    connect(ui->boat_name_edit,SIGNAL(textChanged(QString)),this, SLOT(onTitleChanged(QString)));
    connect(ui->optionEdit,SIGNAL(textChanged(QString)),this, SLOT(onOptionsChanged(QString)));
    connect(ui->videoinfoCheckBox, &QCheckBox::stateChanged,this,&VideoSettingsDialog::onVideoInfoChanged);
    connect(ui->ProxyCheckBox, &QCheckBox::stateChanged,this,&VideoSettingsDialog::onProxyChanged);

}

void VideoSettingsDialog::setInfo(VWSetting settings)
{
    __video_info = settings.video_info;

    ui->titleLineEdit->setText(settings.title);
    ui->videonoComboBox->setCurrentIndex(settings.videono);
    ui->formatComboBox->setCurrentIndex(settings.formatno);

    if(settings.video_info){
        ui->videoinfoCheckBox->setChecked(true);
    }else{
        ui->videoinfoCheckBox->setChecked(false);
    }

    qDebug()<<"setInfo";

}

void VideoSettingsDialog::setFormat(QAbstractItemModel* model, int index)
{
    ui->formatComboBox->setModel(model);
    ui->formatComboBox->setCurrentIndex(index);

}

void VideoSettingsDialog::setVideo(QAbstractItemModel *model, int index)
{
    ui->videonoComboBox->setModel(model);
    ui->videonoComboBox->setCurrentIndex(index);
}


VideoSettingsDialog::~VideoSettingsDialog()
{
    delete ui;
}

void VideoSettingsDialog::accept()
{
    VWSetting settings;
    settings.title = ui->titleLineEdit->text();
    settings.videono =ui->videonoComboBox->currentIndex();
    settings.formatno =ui->formatComboBox->currentIndex();
    settings.video_info = __video_info;
    settings.proxy = proxy;
    emit comit(settings);

    QDialog::accept();
}

void VideoSettingsDialog::onTitleChanged(QString title)
{
    __boatname = title;
}






void VideoSettingsDialog::onOptionsChanged(QString options)
{
}



void VideoSettingsDialog::onPCPortChanged(QString text)
{
    __PCPort = text.toInt();
}


void VideoSettingsDialog::onVideoInfoChanged(int i)
{
    __video_info = ui->videoinfoCheckBox->isChecked();
}

QString VideoSettingsDialog::title()
{
    return __boatname;
}

void VideoSettingsDialog::onProxyChanged(int i)
{
    proxy = ui->ProxyCheckBox->isChecked();
}
