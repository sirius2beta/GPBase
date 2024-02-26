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
    // initialize settings
    settings = new QSettings("Ezosirius", "GPlayer_v1",this);

    //init gpbcore
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
    act = new QAction(tr("view"), this);
    act->setIcon(QIcon(":/icon/view-07.png"));
    ui->toolBar->addAction(act);
    act = new QAction(tr("control"), this);
    act->setIcon(QIcon(":/icon/control-03.png"));
    ui->toolBar->setIconSize(QSize(40, 40));
    ui->toolBar->addAction(act);

    initBoatSettings();
    initVideoWindows();   
    //initSensorWidget();
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
    qDebug()<<"count:"<<gpbcore->videoManager()->count();
    for(int i = 0; i < gpbcore->videoManager()->count(); i++){
        //create settings if first time opened

        VideoWindow* vwindow = addVideoWindow(gpbcore->videoManager()->getVideoItem(i));
        connect(gpbcore->videoManager()->getVideoItem(i), &VideoItem::UIUpdateFormat, vwindow, &VideoWindow::updateFormat);

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
    sensor_widget = new SensorWidget(this);
    sensor_widget->setBoatList(gpbcore->boatManager());
    connect(sensor_widget, &SensorWidget::sendMsg, gpbcore->networkManager(), &NetworkManager::sendMsg);


    dockwidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockwidget->setWidget(sensor_widget);
    dockwidget->setMinimumWidth(400);
    addDockWidget(Qt::LeftDockWidgetArea, dockwidget);
    dockwidget->hide();
    //dockwidget->setFloating(true);

    dockwidget->toggleViewAction()->setIcon(QIcon(":/icon/sensor-02.png"));
    ui->toolBar->addAction(dockwidget->toggleViewAction());
}

VideoWindow* MainWindow::addVideoWindow(VideoItem *videoItem)
{
    Qt::DockWidgetArea area = Qt::RightDockWidgetArea;

    VideoWindow* vwindow = new VideoWindow(this, _config, gpbcore);

    if(settings->value(QString("%1/w%2/videoinfo").arg(_config,QString::number(videoItem->index()))) == 1){
        vwindow->setVideoInfo(true);
    }else{
        vwindow->setVideoInfo(false);
    }
    vwindow->init(videoItem);
    vwindow->setFormat();

    connect(vwindow,&VideoWindow::sendMsg,gpbcore->networkManager(),&NetworkManager::sendMsg);
    //connect(gpbcore->networkManager(), &NetworkManager::setFormat, vwindow, &VideoWindow::setVideoFormat);
    connect(gpbcore, &GPBCore::connectionChanged, vwindow, &VideoWindow::onConnectionChanged);

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









