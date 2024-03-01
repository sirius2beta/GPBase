#include "sensorlabel.h"
#include "ui_sensorlabel.h"
#include <QStyleOption>
#include <QPainter>

SensorLabel::SensorLabel(QWidget *parent, SensorItem *sensorItem) :
    QWidget(parent),
    ui(new Ui::SensorLabel),
    _sensorItem(sensorItem)
{
    ui->setupUi(this);
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
}

void SensorLabel::onSetText(QString text)
{
    ui->value->setText(text);
}

void SensorLabel::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
