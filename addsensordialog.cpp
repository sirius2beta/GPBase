#include "addsensordialog.h"
#include "ui_addsensordialog.h"
#include "gpbcore.h"

AddSensorDialog::AddSensorDialog(QWidget *parent, GPBCore* core) :
    QDialog(parent),
    ui(new Ui::AddSensorDialog),
    _core(core)
{
    ui->setupUi(this);
    ui->boatIDComboBox->setModel(_core->boatManager()->model());
    ui->typeComboBox->setModel(_core->sensorManager()->sensorTypeModel());
}



AddSensorDialog::~AddSensorDialog()
{
    delete ui;
}

void AddSensorDialog::accept()
{
    SensorItem* b = new SensorItem(_core->sensorManager());
    b->setName(ui->sensorNameLineEdit->text());

    emit comit(b);

    QDialog::accept();
}
