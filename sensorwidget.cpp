#include "sensorwidget.h"
#include "ui_sensorwidget.h"

#include "gpbcore.h"
#include "addsensordialog.h"

SensorWidget::SensorWidget(QWidget *parent, GPBCore* core) :
    QWidget(parent),
    ui(new Ui::sensorWidget),
    connection_counter(0),
    costumSensorCount(0),
    _core(core)
{
    ui->setupUi(this);
    connect(ui->info_Button, &QPushButton::clicked, this, &SensorWidget::info_button_pushed);
    connect(ui->addButton, &QPushButton::clicked, this, &SensorWidget::onAddSensorButtonClicked);
    _sensorManager = _core->sensorManager();
    for(int i = 0; i < _sensorManager->count(); i++){
        SensorItem* sensorItem = _sensorManager->getSensor(i);
        addWidget(sensorItem);
    }

}

SensorWidget::~SensorWidget()
{
    delete ui;
}

void SensorWidget::addWidget(SensorItem* sensorItem)
{

    int H = costumSensorCount/3;
    int W = costumSensorCount%3;
    SensorLabel* s = new SensorLabel(this);
    ui->gridLayout->addWidget(s,H,W);
    s->setSensorItem(sensorItem);
    s->setValue("0");

    costumSensorCount++;
}

void SensorWidget::info_button_pushed()
{
    //emit sendMsg(QString("usv1"),QString("qformat"));
}

void SensorWidget::subscribeSensor(int BoatID, int BoardID, int PinID, char unit)
{

}

void SensorWidget::onAddSensorButtonClicked()
{
    AddSensorDialog* d = new AddSensorDialog(this, _core);
    connect(d, &AddSensorDialog::comit, this, &SensorWidget::addWidget);
    d->exec();
}
