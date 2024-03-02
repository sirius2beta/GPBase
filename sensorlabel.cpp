#include "sensorlabel.h"
#include "ui_sensorlabel.h"
#include <QStyleOption>
#include <QPainter>

SensorLabel::SensorLabel(QWidget *parent, SensorItem *sensorItem) :
    QWidget(parent),
    ui(new Ui::SensorLabel),
    _sensorItem(sensorItem),
    _isPlaying(false),
    _disableAlarmSound(false)
{
    ui->setupUi(this);
    alarm = new QSound("qrc:/sound/philips_red_alarm.wav",this);
    alarm->setLoops(QSound::Infinite);
}

SensorLabel::~SensorLabel()
{
    delete ui;
}

void SensorLabel::setSensorItem(SensorItem* sensorItem)
{
    _sensorItem = sensorItem;
    connect(sensorItem, &SensorItem::textSet, this, &SensorLabel::onSetText);
    ui->sensor_name->setText(sensorItem->name());
    onSetText(QString("30"));
}

void SensorLabel::onSetText(QString text)
{
    if(_sensorItem->isEnableMaxAlarm() && QVariant(text).toFloat() > _sensorItem->maxAlarmValue().toFloat()){
        if(!_isPlaying && !_disableAlarmSound){
            alarm->play();
            _isPlaying = true;
        }
        setStyleSheet(QString("*{ background-color: red;}"));
    }else{
        if(_isPlaying) alarm->stop();
        _isPlaying = false;
        setStyleSheet(QString(""));
    }
    ui->value->setText(text);
}


void SensorLabel::onDisableAlarmSound()
{
    alarm->stop();
    _isPlaying = false;
    _disableAlarmSound = _disableAlarmSound?false:true;

}

void SensorLabel::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
