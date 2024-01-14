#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "videowindow.h"
#include "createwindowdialog.h"
#include "configdialog.h"
#include "QTypes.h"


MainWindow::MainWindow(QWidget *parent, QString config)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _config(config)
{
    ui->setupUi(this);
    this->setCorner(Qt::Corner::BottomLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::Corner::TopLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::Corner::BottomRightCorner, Qt::RightDockWidgetArea);
    this->setCorner(Qt::Corner::TopRightCorner, Qt::RightDockWidgetArea);
    this->setDockOptions( QMainWindow::AnimatedDocks);
    this->setWindowTitle(QString("GPlayer"));



    // Setup toolbar icon
    QAction * act;
    act = new QAction(tr("GPlayer"), this);
    act->setIcon(QIcon(":/icon/GP_logo-04.png"));
    ui->toolBar->addAction(act);

    //ConfigDialog* configDialog = new ConfigDialog(this);
    //connect(configDialog,&ConfigDialog::setconfig,this,&MainWindow::setConfig);
    //configDialog->exec();

    boatList = new BoatManager;
    // initialize settings
    settings = new QSettings("Ezosirius", "GPlayer_v1",this);

    //initialize UDP socket
    networkManager = new NetworkManager(this, boatList);

    initBoatSettings();
    //initialize videowindows
    initVideoWindows();

    initSensorWidget();

    act = new QAction(tr("view"), this);
    act->setIcon(QIcon(":/icon/view-07.png"));
    ui->toolBar->addAction(act);

    act = new QAction(tr("control"), this);
    act->setIcon(QIcon(":/icon/control-03.png"));
    ui->toolBar->setIconSize(QSize(40, 40));
    ui->toolBar->addAction(act);

    //connect(ui->addWindowButton, &QPushButton::clicked, this, &MainWindow::openCreateWindowDialog);
}

void MainWindow::initBoatSettings()
{
    QDockWidget* dockwidget = new QDockWidget(QStringLiteral("無人船設定"),this);
    boatSetting = new BoatSetting(dockwidget);
    connect(boatSetting, &BoatSetting::AddBoat, this, &MainWindow::onNewBoat);
    connect(boatSetting, &BoatSetting::sendMsg, networkManager, &NetworkManager::sendMsg);
    connect(networkManager, &NetworkManager::sensorMsg, boatSetting, &BoatSetting::onMsg);
    connect(boatSetting, &BoatSetting::connectionTypeChanged, this, &MainWindow::onConnectionTypeChanged);


    boatSetting->setconfig(_config);
    boatSetting->initSettings(boatList);


    dockwidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockwidget->setWidget(boatSetting);
    dockwidget->setMinimumWidth(300);
    addDockWidget(Qt::LeftDockWidgetArea, dockwidget);
    //dockwidget->hide();

    dockwidget->toggleViewAction()->setIcon(QIcon(":/icon/boat-06.png"));
    ui->toolBar->addAction(dockwidget->toggleViewAction());

}

void MainWindow::initVideoWindows()
{

    int window_count = 4;
    Qt::DockWidgetArea area = Qt::LeftDockWidgetArea;
    //settings->endGroup();
    for(int i = 1; i <= window_count; i++){
        //create settings if first time opened
        if(settings->value(QString("%1/w%2/in_port").arg(_config,QString::number(i))) == QVariant()){
            QList<QString> formatlist = {"video0", "YUYV", "640-480-15", "nan", "80", "192.168.0.100", "5200"};
            settings->setValue(QString("%1/w%2/boat_name").arg(_config,QString::number(i)),QString("unknown"));
            settings->setValue(QString("%1/w%2/in_port").arg(_config,QString::number(i)),5200+i);
            settings->setValue(QString("%1/w%2/title").arg(_config,QString::number(i)),QString("window%1").arg(i));
            settings->setValue(QString("%1/w%2/videoinfo").arg(_config,QString::number(i)), 1);
            settings->setValue(QString("%1/w%2/formatno").arg(_config,QString::number(i)), 0);
        }

        if(i == 1){ //set to central widget
            addVideoWindow( i, true);
        }else{ // set to dock widget
            addVideoWindow( i, false);
        }
    }
}

void MainWindow::initSensorWidget()
{
    //init sensor panel
    QDockWidget* dockwidget = new QDockWidget("Sensor",this);
    sensor_widget = new sensorWidget(dockwidget);
    sensor_widget->setBoatList(boatList);
    dockwidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockwidget->setWidget(sensor_widget);
    dockwidget->setMinimumWidth(400);
    addDockWidget(Qt::LeftDockWidgetArea, dockwidget);
    dockwidget->hide();
    //dockwidget->setFloating(true);

    dockwidget->toggleViewAction()->setIcon(QIcon(":/icon/sensor-02.png"));
    ui->toolBar->addAction(dockwidget->toggleViewAction());

    connect(sensor_widget, &sensorWidget::sendMsg, networkManager, &NetworkManager::sendMsg);

}

void MainWindow::addVideoWindow(int index, bool central_widget)
{
    Qt::DockWidgetArea area = Qt::RightDockWidgetArea;

    VideoWindow* vwindow = new VideoWindow(this,_config);
    vwindow->setIndex(index);
    vwindow->setPCPort(settings->value(QString("%1/w%2/in_port").arg(_config,QString::number(index))).toInt());
    vwindow->setTitle(settings->value(QString("%1/w%2/title").arg(_config,QString::number(index))).toString());
    vwindow->setVideoNo(settings->value(QString("%1/w%2/videono").arg(_config,QString::number(index))).toInt());
    vwindow->setFormatNo(settings->value(QString("%1/w%2/formatno").arg(_config,QString::number(index))).toInt());
    vwindow->setFormat();
    vwindow->setBoatList(boatList);

    if(settings->value(QString("%1/w%2/videoinfo").arg(_config,QString::number(index))) == 1){
        vwindow->setVideoInfo(true);
    }else{
        vwindow->setVideoInfo(false);
    }
    vwindow->init();

    if(central_widget){
        setCentralWidget(vwindow);
    }else{
        QDockWidget* dockwidget = new QDockWidget(settings->value(QString("%1/w%2/title").arg(_config,QString::number(index))).toString(),this);
        addDockWidget(area, dockwidget);
        //dockwidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
        dockwidget->setWidget(vwindow);
        dockwidget->setMinimumHeight(300);
    }

    connect(vwindow,&VideoWindow::sendMsg,networkManager,&NetworkManager::sendMsg);
    connect(networkManager, &NetworkManager::setFormat, vwindow, &VideoWindow::setVideoFormat);
    connect(this, &MainWindow::connectionChanged, vwindow, &VideoWindow::onConnectionChanged);
    connect(boatSetting, &BoatSetting::AddBoat, vwindow, &VideoWindow::AddBoat);
    connect(boatSetting, &BoatSetting::changeBoatName, vwindow, &VideoWindow::onBoatNameChange);
    connect(boatSetting, &BoatSetting::deleteBoat, vwindow, &VideoWindow::onDeleteBoat);

}

void MainWindow::openCreateWindowDialog()
{
    CreateWindowDialog* dialog = new CreateWindowDialog(this);
    dialog->exec();
}






MainWindow::~MainWindow()
{
    delete ui;
    delete boatList;
}





void MainWindow::setConfig(QString config)
{
    _config = QString("config")+config;
    qDebug()<<config;
}


void MainWindow::onConnected(int ID, bool isprimary)
{

    Boat* boat = boatList->getBoatbyID(ID);
    if(boat != 0){
        if(isprimary){
            boat->CurrentIP = boat->PIP;
            emit connectionChanged();
        }else{
            if(boat->CurrentIP != boat->PIP){
                boat->CurrentIP = boat->SIP;
                emit connectionChanged();
            }
        }
    }


}

void MainWindow::onDisonnected(int ID, bool isprimary)
{

    Boat* boat = boatList->getBoatbyID(ID);
    if(boat != 0){
        if(isprimary){
            boat->CurrentIP = boat->SIP;
            emit connectionChanged();
        }

    }



}

void MainWindow::onNewBoat(Boat* newboat)
{
    primaryHeartBeat = new HeartBeat(newboat, boatList, PCPIP, 50006, true, this);
    qDebug()<<"MainWindow::onNewBoat ck1";
    primaryHeartBeat->HeartBeatLoop();
    qDebug()<<"MainWindow::onNewBoat ck2";
    secondaryHeartBeat = new HeartBeat(newboat, boatList, PCSIP, 50006, false, this);
    secondaryHeartBeat->HeartBeatLoop();
    connect(networkManager, &NetworkManager::AliveResponse, primaryHeartBeat, &HeartBeat::alive);
    connect(primaryHeartBeat, &HeartBeat::sendMsg, networkManager, &NetworkManager::sendMsg);
    connect(primaryHeartBeat, &HeartBeat::connected, boatSetting, &BoatSetting::onConnected);
    connect(primaryHeartBeat, &HeartBeat::disconnected, boatSetting, &BoatSetting::onDisonnected);
    connect(primaryHeartBeat, &HeartBeat::connected, this, &MainWindow::onConnected);
    connect(primaryHeartBeat, &HeartBeat::disconnected, this, &MainWindow::onDisonnected);
    connect(boatSetting, &BoatSetting::changeBoatName, primaryHeartBeat, &HeartBeat::resetBoatName);
    connect(boatSetting, &BoatSetting::ChangeIP, primaryHeartBeat, &HeartBeat::onChangeIP);
    connect(boatSetting, &BoatSetting::deleteBoat, primaryHeartBeat, &HeartBeat::onDeleteBoat);

    connect(networkManager, &NetworkManager::AliveResponse, secondaryHeartBeat, &HeartBeat::alive);
    connect(secondaryHeartBeat, &HeartBeat::sendMsg, networkManager, &NetworkManager::sendMsg);
    connect(secondaryHeartBeat, &HeartBeat::connected, boatSetting, &BoatSetting::onConnected);
    connect(secondaryHeartBeat, &HeartBeat::disconnected, boatSetting, &BoatSetting::onDisonnected);
    connect(secondaryHeartBeat, &HeartBeat::connected, this, &MainWindow::onConnected);
    connect(secondaryHeartBeat, &HeartBeat::disconnected, this, &MainWindow::onDisonnected);
    connect(boatSetting, &BoatSetting::changeBoatName, secondaryHeartBeat, &HeartBeat::resetBoatName);
    connect(boatSetting, &BoatSetting::ChangeIP, secondaryHeartBeat, &HeartBeat::onChangeIP);
    connect(boatSetting, &BoatSetting::deleteBoat, secondaryHeartBeat, &HeartBeat::onDeleteBoat);
}

void MainWindow::onConnectionTypeChanged()
{
    emit connectionChanged();
}


