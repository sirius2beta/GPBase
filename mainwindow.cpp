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

    boatList = new BoatManager;

    // Setup toolbar icon
    QAction * act;
    act = new QAction(tr("GPlayer"), this);
    act->setIcon(QIcon(":/icon/GP_logo-04.png"));
    ui->toolBar->addAction(act);

    //ConfigDialog* configDialog = new ConfigDialog(this);
    //connect(configDialog,&ConfigDialog::setconfig,this,&MainWindow::setConfig);
    //configDialog->exec();

    // initialize settings
    settings = new QSettings("Ezosirius", "GPlayer_v1",this);

    //initialize UDP socket
    serverSocket = new QUdpSocket(this);
    clientSocket = new QUdpSocket(this);
    clientSocket->bind(50008,QUdpSocket::ShareAddress);
    connect(clientSocket,&QUdpSocket::readyRead,this, &MainWindow::onUDPMsg);

    //initialize NetworkSettings
    initNetworkSettings();

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

void MainWindow::initNetworkSettings()
{

    QDockWidget* dockwidget = new QDockWidget(QStringLiteral("電腦端設定"),this);
    networkSettings = new NetworkSettings(dockwidget);
    networkSettings->setConfig(_config);
    connect(networkSettings, &NetworkSettings::PIPChanged, this, &MainWindow::onPCPIPChanged);
    connect(networkSettings, &NetworkSettings::SIPChanged, this, &MainWindow::onPCSIPChanged);

    //dockwidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockwidget->setWidget(networkSettings);
    dockwidget->setMinimumWidth(300);
    dockwidget->setMaximumHeight(170);
    addDockWidget(Qt::LeftDockWidgetArea, dockwidget);

    dockwidget->toggleViewAction()->setIcon(QIcon(":/icon/PC-05.png"));
    ui->toolBar->addAction(dockwidget->toggleViewAction());

    PCPIP = settings->value(QString("%1/PC/PIP").arg(_config)).toString();
    PCSIP = settings->value(QString("%1/PC/SIP").arg(_config)).toString();

    networkSettings->setIP(PCPIP, PCSIP);
}

void MainWindow::initBoatSettings()
{
    QDockWidget* dockwidget = new QDockWidget(QStringLiteral("無人船設定"),this);
    boatSetting = new BoatSetting(dockwidget);
    connect(boatSetting, &BoatSetting::AddBoat, this, &MainWindow::onNewBoat);
    connect(boatSetting, &BoatSetting::sendMsg, this, &MainWindow::sendMsg);
    connect(this, &MainWindow::sensorMsg, boatSetting, &BoatSetting::onMsg);
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

    connect(sensor_widget, &sensorWidget::sendMsg, this, &MainWindow::sendUDPCommand);

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

    connect(vwindow,&VideoWindow::sendCommand,this,&MainWindow::sendUDPCommand);
    connect(vwindow,&VideoWindow::sendMsg,this,&MainWindow::sendMsg);
    connect(this, &MainWindow::setFormat, vwindow, &VideoWindow::setVideoFormat);
    connect(this, &MainWindow::connectionChanged, vwindow, &VideoWindow::onConnectionChanged);
    connect(boatSetting, &BoatSetting::vAddBoat, vwindow, &VideoWindow::AddBoat);
    connect(boatSetting, &BoatSetting::changeBoatName, vwindow, &VideoWindow::onBoatNameChange);
    connect(boatSetting, &BoatSetting::deleteBoat, vwindow, &VideoWindow::onDeleteBoat);

}

void MainWindow::openCreateWindowDialog()
{
    CreateWindowDialog* dialog = new CreateWindowDialog(this);
    dialog->exec();
}

void MainWindow::sendUDPCommand(int ID, QString command, int PCPort)
{
    //QHostAddress addr = QHostAddress(boatSetting->getIP(boatname));

    Boat* boat = boatList->getBoatbyID(ID);
    if(boat == 0){
        qDebug()<<"MainWindow::sendUDPCommand:no boatname in boatlist";
        return;
    }
    QHostAddress addr;
    QString PC_addr;
    if(boat)
    if(boatSetting->connectionType() == BoatSetting::Auto){
         addr = QHostAddress(boat->CurrentIP);

         if(boatSetting->isPrimary(ID)){
             PC_addr = PCPIP;
         }else{
             PC_addr = PCSIP;
         }
    }else if(boatSetting->connectionType() == BoatSetting::Primary){
            addr = QHostAddress(boat->PIP);
            PC_addr = PCPIP;

    }else{

            addr = QHostAddress(boat->SIP);
            PC_addr = PCSIP;

    }




    if(PCPort != 0){
        command = command+" "+PC_addr+" "+QString::number(PCPort);
    }
    sendMsg(addr, char(COMMAND), QByteArray::fromStdString(command.toStdString()));
    qDebug()<<"MainWindow "<<",ip: "<<addr.toString()<<"send command: "<<command;

}


void MainWindow::sendMsg(QHostAddress addr, char topic, QByteArray command)
{


    qDebug()<<"Mainwindow: sendMsg to :"<<addr.toString();

    QByteArray cmd;
    cmd.resize(1);
    cmd[0] = topic;
    //cmd[1] = ' ';
    cmd.append(command);
    serverSocket->writeDatagram(cmd,cmd.size(), addr, 50006);

}



MainWindow::~MainWindow()
{
    delete ui;
    delete boatList;
}



void MainWindow::onUDPMsg()
{
    while(clientSocket->hasPendingDatagrams()){
        QByteArray data;
        QHostAddress addr;
        QString ip;
        data.resize(clientSocket->pendingDatagramSize());
        clientSocket->readDatagram(data.data(),data.size(),&addr);

        char topic = data[0];
        data.remove(0,1);

        ip = QHostAddress(addr.toIPv4Address()).toString();
        if(ip == BoatPIP){
            //qDebug()<<QString("recv from primary: %1").arg(ip);
        }else if(ip == BoatSIP){
            //qDebug()<<QString("recv from secondary: %1").arg(ip);
        }

        QStringList dataList = QString(data).split(' ');

        QString message;
        if(data.split(' ').size() >1){
            message = data.split(' ')[1];
        }
        if(topic == HEARTBEAT){
            int ID = int(data[0]);
            Boat* boat = boatList->getBoatbyID(ID);
            if( boat != 0){
                emit AliveResponse(ip);
            }


        }else if(topic == FORMAT){
            int ID = int(data[0]);
            QString format = data.remove(0,1);
            qDebug()<<"MainWindow call from FORMAT, boat ID:"<<ID;
            qDebug()<<format;
            if(format != ""){
                emit setFormat(ID, format.split('\n'));
            }

        }else if(topic == SENSOR){
            qDebug()<<"sensor";
            emit sensorMsg(data);
        }
        const QString content = QLatin1String(" Received Topic: ")
                    + topic
                    + QLatin1String(" Message: ")
                    + message
                    + QLatin1Char('\n');
        //qDebug() << content;
    }
}

void MainWindow::setConfig(QString config)
{
    _config = QString("config")+config;
    qDebug()<<config;
}

void MainWindow::onPCPIPChanged(QString PCIP)
{
    PCPIP = PCIP;
    emit changePCPIP(PCIP);
    settings->setValue(QString("%1/PC/PIP").arg(_config), PCIP);

}

void MainWindow::onPCSIPChanged(QString PCIP)
{
    PCSIP = PCIP;
    emit changePCSIP(PCIP);
    settings->setValue(QString("%1/PC/SIP").arg(_config), PCIP);
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

void MainWindow::onNewBoat(QString boatname, int ID, QString PIP, QString SIP)
{
    primaryHeartBeat = new HeartBeat(boatname, ID, PCPIP,PIP, 50006,true, this);
    primaryHeartBeat->HeartBeatLoop();
    secondaryHeartBeat = new HeartBeat(boatname, ID, PCSIP,SIP, 50006,false, this);
    secondaryHeartBeat->HeartBeatLoop();
    connect(this, &MainWindow::AliveResponse, primaryHeartBeat, &HeartBeat::alive);
    connect(primaryHeartBeat, &HeartBeat::sendMsg, this, &MainWindow::sendMsg);
    connect(primaryHeartBeat, &HeartBeat::connected, boatSetting, &BoatSetting::onConnected);
    connect(primaryHeartBeat, &HeartBeat::disconnected, boatSetting, &BoatSetting::onDisonnected);
    connect(primaryHeartBeat, &HeartBeat::connected, this, &MainWindow::onConnected);
    connect(primaryHeartBeat, &HeartBeat::disconnected, this, &MainWindow::onDisonnected);
    connect(boatSetting, &BoatSetting::changeBoatName, primaryHeartBeat, &HeartBeat::resetBoatName);
    connect(boatSetting, &BoatSetting::ChangeIP, primaryHeartBeat, &HeartBeat::onChangeIP);
    connect(boatSetting, &BoatSetting::deleteBoat, primaryHeartBeat, &HeartBeat::onDeleteBoat);
    connect(this, &MainWindow::changePCPIP, primaryHeartBeat, &HeartBeat::onPCPIPChanged);

    connect(this, &MainWindow::AliveResponse, secondaryHeartBeat, &HeartBeat::alive);
    connect(secondaryHeartBeat, &HeartBeat::sendMsg, this, &MainWindow::sendMsg);
    connect(secondaryHeartBeat, &HeartBeat::connected, boatSetting, &BoatSetting::onConnected);
    connect(secondaryHeartBeat, &HeartBeat::disconnected, boatSetting, &BoatSetting::onDisonnected);
    connect(secondaryHeartBeat, &HeartBeat::connected, this, &MainWindow::onConnected);
    connect(secondaryHeartBeat, &HeartBeat::disconnected, this, &MainWindow::onDisonnected);
    connect(boatSetting, &BoatSetting::changeBoatName, secondaryHeartBeat, &HeartBeat::resetBoatName);
    connect(boatSetting, &BoatSetting::ChangeIP, secondaryHeartBeat, &HeartBeat::onChangeIP);
    connect(boatSetting, &BoatSetting::deleteBoat, secondaryHeartBeat, &HeartBeat::onDeleteBoat);
    connect(this, &MainWindow::changePCSIP, secondaryHeartBeat, &HeartBeat::onPCSIPChanged);
}

void MainWindow::onConnectionTypeChanged()
{
    emit connectionChanged();
}


