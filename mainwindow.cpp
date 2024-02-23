#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "createwindowdialog.h"
#include "configdialog.h"
#include "QTypes.h"


MainWindow::MainWindow(QWidget *parent, QString config)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _config(config)
{
    gpbcore = new GPBCore(this, config);
    gpbcore->init();

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


    // initialize settings
    settings = new QSettings("Ezosirius", "GPlayer_v1",this);

    initBoatSettings();
    initVideoWindows();
    //initSensorWidget();

    act = new QAction(tr("view"), this);
    act->setIcon(QIcon(":/icon/view-07.png"));
    ui->toolBar->addAction(act);

    act = new QAction(tr("control"), this);
    act->setIcon(QIcon(":/icon/control-03.png"));
    ui->toolBar->setIconSize(QSize(40, 40));
    ui->toolBar->addAction(act);
}

void MainWindow::initBoatSettings()
{
    QDockWidget* dockwidget = new QDockWidget(QStringLiteral("無人船設定"),this);
    BoatSetting* boatSetting = gpbcore->boatSetting();

    dockwidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockwidget->setWidget(boatSetting);
    dockwidget->setMinimumWidth(300);
    addDockWidget(Qt::LeftDockWidgetArea, dockwidget);

    dockwidget->toggleViewAction()->setIcon(QIcon(":/icon/boat-06.png"));
    ui->toolBar->addAction(dockwidget->toggleViewAction());

}

void MainWindow::initVideoWindows()
{

    int window_count = 4;
    Qt::DockWidgetArea area = Qt::RightDockWidgetArea;
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


        VideoWindow* vwindow = addVideoWindow(i);


        if(i == 1){
            setCentralWidget(vwindow);
        }else{
            QDockWidget* dockwidget = new QDockWidget(settings->value(QString("%1/w%2/title").arg(_config,QString::number(i))).toString(),this);
            addDockWidget(area, dockwidget);
            //dockwidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
            dockwidget->setWidget(vwindow);
            dockwidget->setMinimumHeight(300);
        }
    }
}

void MainWindow::initSensorWidget()
{
    //init sensor panel
    QDockWidget* dockwidget = new QDockWidget("Sensor",this);
    sensor_widget = gpbcore->sensorWidget();

    dockwidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockwidget->setWidget(sensor_widget);
    dockwidget->setMinimumWidth(400);
    addDockWidget(Qt::LeftDockWidgetArea, dockwidget);
    dockwidget->hide();
    //dockwidget->setFloating(true);

    dockwidget->toggleViewAction()->setIcon(QIcon(":/icon/sensor-02.png"));
    ui->toolBar->addAction(dockwidget->toggleViewAction());



}

VideoWindow* MainWindow::addVideoWindow(int index)
{
    Qt::DockWidgetArea area = Qt::RightDockWidgetArea;

    VideoWindow* vwindow = new VideoWindow(this, _config, gpbcore);
    vwindow->setIndex(index);
    vwindow->setPCPort(settings->value(QString("%1/w%2/in_port").arg(_config,QString::number(index))).toInt());
    vwindow->setTitle(settings->value(QString("%1/w%2/title").arg(_config,QString::number(index))).toString());
    vwindow->setVideoNo(settings->value(QString("%1/w%2/videono").arg(_config,QString::number(index))).toInt());
    vwindow->setFormatNo(settings->value(QString("%1/w%2/formatno").arg(_config,QString::number(index))).toInt());
    vwindow->setFormat();

    if(settings->value(QString("%1/w%2/videoinfo").arg(_config,QString::number(index))) == 1){
        vwindow->setVideoInfo(true);
    }else{
        vwindow->setVideoInfo(false);
    }
    vwindow->init();



    connect(vwindow,&VideoWindow::sendMsg,gpbcore->networkManager(),&NetworkManager::sendMsg);
    connect(gpbcore->networkManager(), &NetworkManager::setFormat, vwindow, &VideoWindow::setVideoFormat);
    connect(gpbcore, &GPBCore::connectionChanged, vwindow, &VideoWindow::onConnectionChanged);
    connect(gpbcore->boatManager(), &BoatManager::boatAdded, vwindow, &VideoWindow::AddBoat);
    connect(gpbcore->boatManager(), &BoatManager::boatDeleted, vwindow, &VideoWindow::onDeleteBoat);

    return vwindow;

}

void MainWindow::openCreateWindowDialog()
{
    CreateWindowDialog* dialog = new CreateWindowDialog(this);
    dialog->exec();
}






MainWindow::~MainWindow()
{
    delete ui;

}





void MainWindow::setConfig(QString config)
{
    _config = QString("config")+config;
    qDebug()<<config;
}









