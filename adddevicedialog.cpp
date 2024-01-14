#include "adddevicedialog.h"
#include "setpindialog.h"
#include "ui_adddevicedialog.h"

AddDeviceDialog::AddDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDeviceDialog),
    backgrounWidget(0),
    currentMode(-1),
    devName(QString("unknown"))
{
    ui->setupUi(this);

    connect(ui->SensorButton, &QPushButton::clicked, this, &AddDeviceDialog::onAddSensor);
    connect(ui->ServoButton, &QPushButton::clicked, this, &AddDeviceDialog::onAddServo);
    connect(ui->StepperButton, &QPushButton::clicked, this, &AddDeviceDialog::onAddStepper);


}

AddDeviceDialog::~AddDeviceDialog()
{
    delete ui;
}

void AddDeviceDialog::setPeripheralModel(QStandardItemModel* pm, Boat* boat){
    thisBoat = boat;
    this->peripheralItemModel = pm;
    newPeripherals = boat->peripherals;
    oldPeripherals = boat->peripherals;

    connect(ui->SensorButton, &QPushButton::clicked, this, &AddDeviceDialog::onAddSensor);
    connect(ui->ServoButton, &QPushButton::clicked, this, &AddDeviceDialog::onAddServo);

}

QList<Peripheral> AddDeviceDialog::getPeripherals()
{
    return newPeripherals;
}

Device AddDeviceDialog::getDevice()
{
    newDev.deviceName = devName;
    newDev.pinUIDList.clear();

    for(int i = 0; i<pinCurrentSelected.size(); i++){

        if(pinCurrentSelected[i].size() == 0){
            newDev.pinUIDList.clear();
            qDebug()<<i;
            return newDev;
        }
        for(int j = 0; j<pinCurrentSelected[i].size(); j++){

            newDev.addPin(pinCurrentSelected[i][j]);
        }
    }
    qDebug()<<"+"<<newDev.pinUIDList.size();
    return newDev;
}

QString AddDeviceDialog::getDevName(){
    return devName;
}

QWidget* AddDeviceDialog::addDevNameField()
{
    QWidget* holderwidget = new QWidget(backgrounWidget);
    QHBoxLayout* layout = new QHBoxLayout(holderwidget);
    layout->setContentsMargins(11,2,11,2);
    QLineEdit* lineEdit = new QLineEdit(holderwidget);
    lineEdit->setText(devName);
    QLabel* label = new QLabel(holderwidget);
    label->setText("Device name");
    layout->addWidget(label);

    layout->addWidget(lineEdit);
    layout->addStretch();
    backgrounWidget->layout()->addWidget(holderwidget);
    connect(lineEdit, &QLineEdit::textChanged, this, &AddDeviceDialog::onDevNameChanged);
    return holderwidget;
}

QWidget* AddDeviceDialog::addPinSelectField(QString name)
{
    QWidget* holderwidget = new QWidget(backgrounWidget);
    QHBoxLayout* layout = new QHBoxLayout(holderwidget);
    QLineEdit* lineEdit = new QLineEdit(holderwidget);
    selectedPinLineEdit = lineEdit;

    QPushButton* button = new QPushButton("...", holderwidget);
    QLabel* label = new QLabel(holderwidget);
    pinLineEditList.append(lineEdit);
    pinPushButton.append(button);
    pinCurrentSelected.append(QList<int> {});
    label->setText(name);
    layout->addWidget(label);

    layout->addWidget(lineEdit);
    layout->addWidget(button);
    layout->addStretch();
    backgrounWidget->layout()->addWidget(holderwidget);
    connect(button, &QPushButton::clicked, this, &AddDeviceDialog::onSelectPins);
    return holderwidget;
}

void AddDeviceDialog::onAddSensor()
{
    newDev = Device();
    newDev.type = Device::SENSOR;

    newPeripherals = oldPeripherals;

    pinLineEditList.clear();
    pinPushButton.clear();
    pinCurrentSelected.clear();

    currentMode = Device::SENSOR;
    if(backgrounWidget != 0){
        backgrounWidget->deleteLater();
    }
    backgrounWidget = new QWidget(this);
    ui->OptionLayout->addWidget(backgrounWidget);


    QVBoxLayout* layout = new QVBoxLayout(backgrounWidget);
    backgrounWidget->setLayout(layout);
    addDevNameField();
    addPinSelectField("Sensor Pin");


    qobject_cast<QVBoxLayout*>(backgrounWidget->layout())->addStretch();
    currentMode = Device::SENSOR;



}

void AddDeviceDialog::onAddServo()
{
    //init newDev
    newDev = Device();
    currentMode = Device::SERVO;

    newPeripherals = oldPeripherals;


    if(backgrounWidget != 0){
        backgrounWidget->deleteLater();
    }
    backgrounWidget = new QWidget(this);
    ui->OptionLayout->addWidget(backgrounWidget);



    QVBoxLayout* layout = new QVBoxLayout(backgrounWidget);
    backgrounWidget->setLayout(layout);
    addDevNameField();
    addPinSelectField("Servo Pin");

    qobject_cast<QVBoxLayout*>(backgrounWidget->layout())->addStretch();
    /*
    SetPinDialog* dialog = new SetPinDialog(this);
    dialog->setPeripheralModel(peripheralItemModel, thisBoat);
    dialog->setType(Device::SERVO);
    int ret = dialog->exec();
    if(ret==QDialog::Accepted){
        thisBoat->peripherals = dialog->getPeripherals();
    }
    */
}

void AddDeviceDialog::onAddStepper()
{
    newDev = Device();
    newDev.type = Device::STEPPER;

    newPeripherals = oldPeripherals;

    pinLineEditList.clear();
    pinPushButton.clear();
    pinCurrentSelected.clear();

    currentMode = Device::STEPPER;
    if(backgrounWidget != 0){
        backgrounWidget->deleteLater();
    }
    backgrounWidget = new QWidget(this);
    ui->OptionLayout->addWidget(backgrounWidget);

    QVBoxLayout* layout = new QVBoxLayout(backgrounWidget);
    backgrounWidget->setLayout(layout);
    addDevNameField();
    addPinSelectField("Pin1");
    addPinSelectField("Pin2");


    qobject_cast<QVBoxLayout*>(backgrounWidget->layout())->addStretch();
    currentMode = Device::STEPPER;



}

void AddDeviceDialog::onSelectPins()
{
    int maxnum = 1;
    if(currentMode == -1){
        return;
    }
    if(currentMode == Device::SENSOR){
        maxnum = 1;
    }else if(currentMode == Device::SERVO){
        maxnum = 1;
    }else{
        maxnum = 1;
    }
    int index = 0;
    for(int i = 0; i<pinPushButton.size(); i++){
        if(pinPushButton[i] == sender()){
            qDebug()<<"index:"<<i;
            index = i;
        }
    }

    SetPinDialog* dialog = new SetPinDialog(this);
    dialog->setMaxNum(maxnum);
    dialog->setPeripheralModel(peripheralItemModel, thisBoat);
    dialog->setPeripherals(newPeripherals);
    dialog->setDev(newDev);
    dialog->setType(currentMode);
    dialog->setIndex(index);
    //create free ID
    int count = 0;

    QVector<bool> IDFree(256, true);
    int newID = 0;
    for(int i = 0; i < thisBoat->devices.size();i++){
        IDFree[thisBoat->devices[i].ID] = false;
    }

    for(int i =0; i<256; i++){
        if(IDFree[i] == true){
            newID = i;
            break;
        }
    }

    int ret = dialog->exec();
    if(ret==QDialog::Accepted){
        QString line;

        newPeripherals = dialog->getPeripherals();
        newDev = dialog->getDevice();
        newDev.ID = newID;
        pinCurrentSelected[index] = dialog->getpinCurrentSelected();
        for(int i = 0; i< pinCurrentSelected[index].size(); i++){
            line+=QString::number(pinCurrentSelected[index][i]/256);
            line+=":";
            line+=QString::number(pinCurrentSelected[index][i]%256);
            line+=", ";
        }
        pinLineEditList[index]->setText(line);

    }
}

void AddDeviceDialog::onDevNameChanged(const QString& n)
{
    devName = n;
}
