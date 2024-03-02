#include "boatsetting.h"
#include "ui_boatsetting.h"
#include <QComboBox>
#include <QMessageBox>
#include "gpbcore.h"
#define SENSOR 0x50

BoatSetting::BoatSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoatSetting),
    initialized(false)
{
    ui->setupUi(this);

    ui->primaryButton->setChecked(true);

    ui->BoatTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->BoatTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->BoatlineEdit, &QLineEdit::editingFinished, this, &BoatSetting::onBoatNameChange);
    connect(ui->BoatTableView, &QTableView::clicked, this, &BoatSetting::onBoatDoubleClicked);
    connect(ui->PIPlineEdit, &QLineEdit::editingFinished, this, &BoatSetting::onChangePIP);
    connect(ui->SIPlineEdit, &QLineEdit::editingFinished, this, &BoatSetting::onChangeSIP);
    ui->BoatTableView->verticalHeader()->setDefaultSectionSize(30);
    ui->DeviceTableView->verticalHeader()->setDefaultSectionSize(10);
    //setup boat tableview
    ui->BoatTableView->verticalHeader()->setVisible(false);

    deviceItemModel = new QStandardItemModel();
    QStringList label2 = {"ID", QStringLiteral("USB裝置"), QStringLiteral("腳位"),QStringLiteral("類別"),QStringLiteral("狀態")};
    deviceItemModel->setHorizontalHeaderLabels(label2);
    ui->DeviceTableView->setModel(deviceItemModel);
    ui->DeviceTableView->setColumnWidth(0,30);
    ui->DeviceTableView->setColumnWidth(1,100);
    ui->DeviceTableView->setColumnWidth(2,100);
    ui->DeviceTableView->setColumnWidth(3,80);
    ui->DeviceTableView->verticalHeader()->setVisible(false);


    peripheralItemModel = new QStandardItemModel();
    QStringList label3 = {"ID", QStringLiteral("USB裝置"), QStringLiteral("類別"),QStringLiteral("狀態")};
    peripheralItemModel->setHorizontalHeaderLabels(label3);
    ui->PeriTableView->setModel(peripheralItemModel);
    ui->PeriTableView->setColumnWidth(0,50);
    ui->PeriTableView->setColumnWidth(2,100);
    ui->PeriTableView->setColumnWidth(3,50);
    ui->PeriTableView->setStyleSheet("QTableView::indicator:checked { background-color: #55ee55;} QTableView::indicator:unchecked { background-color: #ee5555;}");
    ui->PeriTableView->verticalHeader()->setVisible(false);

    connect(ui->addBoatPushButton, &QPushButton::clicked, this, &BoatSetting::onAddBoat);
    connect(ui->deleteBoatPushButton, &QPushButton::clicked, this, &BoatSetting::onDeleteBoat);
    connect(ui->BoatcomboBox, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, this, &BoatSetting::onBoatSelected);
    connect(ui->addDeviceButton, &QPushButton::clicked, this, &BoatSetting::onAddDeviceButtonClicked);
    connect(ui->primaryButton, &QPushButton::clicked, this, &BoatSetting::setPrimaryPriority);
    connect(ui->secondaryButton, &QPushButton::clicked, this, &BoatSetting::setSecondaryPriority);
}

void BoatSetting::init(GPBCore* core)
{
    boatManager = core->boatManager();
    ui->BoatTableView->setModel(boatManager->model());
    ui->BoatTableView->setColumnWidth(1,80);
    ui->BoatTableView->setColumnWidth(2,80);
    int size = boatManager->size();
    connect(this, &BoatSetting::connectionTypeChanged, boatManager, &BoatManager::onConnectionTypeChanged);

    if(size==0){
        ui->infoBox->setVisible(false);
    }else{

        for(int i = 0; i<size; i++){
            BoatItem* boat = boatManager->getBoatbyIndex(i);
            ui->BoatTableView->setRowHeight(boatManager->size(),30);
            ui->BoatcomboBox->addItem(boat->name(),boat->name());
            ui->BoatcomboBox->setCurrentIndex(ui->BoatcomboBox->count()-1);
            ui->BoatlineEdit->setText(boat->name());
            ui->PIPlineEdit->setText(boat->PIP());
            ui->SIPlineEdit->setText(boat->SIP());
        }

    }
    initialized = true;
}

void BoatSetting::addBoard(int ID, QString boardName, QString boardType, bool connected)
{
    int row = peripheralItemModel->rowCount();
    QStandardItem* item1 = new QStandardItem(QString::number(ID));
    item1->setEditable(false);
    item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QStandardItem* item2 = new QStandardItem(boardName);
    QStandardItem* item3 = new QStandardItem(boardType);
    item3->setEditable(false);
    QStandardItem* item4 = new QStandardItem(true);
    item4->setEditable(false);
    item4->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item4->setCheckable(false);
    if(connected){
        item4->setCheckState(Qt::Checked);
    }else{
        item4->setCheckState(Qt::Unchecked);
    }

    peripheralItemModel->setItem(row,0,item1);
    peripheralItemModel->setItem(row,1,item2);
    peripheralItemModel->setItem(row,2,item3);
    peripheralItemModel->setItem(row,3,item4);
    ui->PeriTableView->setRowHeight(row,5);

}

void BoatSetting::upDateDeviceTableView()
 {
    /*
     for(int i = 0; i < currentBoat()->devices.size(); i++){
         QStandardItem* item1 = new QStandardItem(QString::number(currentBoat()->devices[i].ID));
         QStandardItem* item2 = new QStandardItem(currentBoat()->devices[i].deviceName);
         QString line;

         for(int j = 0; j< currentBoat()->devices[i].pinUIDList.size(); j++){
             line+=QString::number(currentBoat()->devices[i].boardofPin(j));
             line+=":";
             line+=QString::number(currentBoat()->devices[i].pinIDofPin(j));
             line+=", ";
         }
         QStandardItem* item3 = new QStandardItem(line);

         QStandardItem* item4 = new QStandardItem(currentBoat()->devices[i].typeName());
         deviceItemModel->setItem(i,0, item1);
         deviceItemModel->setItem(i,1, item2);
         deviceItemModel->setItem(i,2, item3);
         deviceItemModel->setItem(i,3, item4);
     }
*/
 }

BoatSetting::~BoatSetting()
{
    delete ui;
}

void BoatSetting::addDevice(int ID, QString Devicename, char format, bool IO)
{
    int current = deviceItemModel->rowCount();
    QStandardItem* item1 = new QStandardItem(Devicename);
    QStandardItem* item2 = new QStandardItem(format);
    QStandardItem* item3;
    if(IO){
         item3 = new QStandardItem(QString("I"));
    }else{
         item3 = new QStandardItem(QString("O"));
    }
    /*
    boatItemModel->setItem(current,0,item1);
    boatItemModel->setItem(current,1,item2);
    boatItemModel->setItem(current,2,item3);
    */
}

void BoatSetting::deleteDevice(int ID)
{

}

/*
void BoatSetting::onMsg(QByteArray data)
{

    int ID = int(data[0]);
    char mode = char(data[1]);
    data.remove(0,1);
    QStringList devlistline = QString(data).split('\n');

    if(mode == 'r'){
        if(ID != boatManager->getBoatbyIndex(ui->BoatcomboBox->currentIndex())->ID()){
            return;
        }
        if(!initialized){
            return;
        }
        qDebug()<<devlistline.size();
        for(int i = 0; i < devlistline.size(); i++){
            if(devlistline[i].size() == 0){
                break;
            }
            qDebug()<<devlistline[i];

            QStringList devinfo = devlistline[i].split(',');
            int devID = devinfo[4].toInt();
            bool cont = false;
            for(int i = 0; i<peripheralItemModel->rowCount(); i++){
                if(devID ==peripheralItemModel->item(i,0)->text().toInt()){
                    cont = true;
                    peripheralItemModel->item(i,3)->setCheckState(Qt::Checked);
                }else{

                }
                qDebug()<<"---------"<<peripheralItemModel->item(i,0)->text().toInt()<<", "<<devID;
            }


            if(!cont){
                Peripheral newperiperal;
                BoatItem* boat = boatManager->getBoatbyIndex(ui->BoatcomboBox->currentIndex());
                newperiperal.ID = devinfo[4].toInt();

                newperiperal.boardName = devinfo[2];

                QString dev_type = (newperiperal.boardName).split(" ")[0];
                newperiperal.boardType = dev_type;
                if(dev_type == QString("FTDI")){
                    dev_type = QString("RS485_hub");
                }else if(dev_type == QString("Silicon")){
                    dev_type = QString("Gyro");
                }
                boat->peripherals.append(newperiperal);
                qDebug()<<"--------Add periperal-----------";
                qDebug()<<"boat:"<<boat->name();
                qDebug()<<"DevID"<<newperiperal.ID;
                qDebug()<<"DevName"<<newperiperal.boardName;
                qDebug()<<"DevType"<<newperiperal.boardType;


                addBoard(newperiperal.ID, newperiperal.boardName, dev_type, true);
                currentBoat()->peripherals.append(newperiperal);

                settings->beginGroup(QString("%1").arg(config));
                int size = settings->beginReadArray(QString("boat/%1/board").arg(ui->BoatcomboBox->currentIndex()+1));

                settings->endArray();
                settings->beginWriteArray(QString("boat/%1/board").arg(ui->BoatcomboBox->currentIndex()+1));
                settings->setArrayIndex(size);
                settings->setValue(QString("ID"), newperiperal.ID);
                settings->setValue(QString("boardName"), newperiperal.boardName);
                settings->setValue(QString("boardType"), dev_type);
                settings->endArray();
                settings->endGroup();

            }




        }

    }

    int sensor_num = data.mid(1,1).toHex().toInt(&ok);
    int start = 1;
    for(int i = 0; i<sensor_num; i++){
        QString unit = data.mid(start,1);
        int BoatID = data.mid(start+1,1).toHex().toInt(&ok);
        int DeviceID = data.mid(start+2,1).toHex().toInt(&ok);
        int uid = 255*BoatID + DeviceID;
        qDebug()<<"Sensor uid:"<<uid;
        SensorLabel* label = labelMap[uid];
        if(label == 0){
            return;
        }
        if(label->unit == 'i'){
            bool ok;
            int value = data.mid(start+3,4).toHex().toInt(&ok, 16);
            label->setValue(QString::number(value));
            start+=7;
        }
    }



}
*/

void BoatSetting::onBoatNameChange()
{

    BoatItem* boat = boatManager->getBoatbyIndex(ui->BoatcomboBox->currentIndex());
    QString newname = ui->BoatlineEdit->text();
    boat->setName(newname);
    ui->BoatcomboBox->setItemText(ui->BoatcomboBox->currentIndex(),ui->BoatlineEdit->text());
    qDebug()<<"namec";

}

void BoatSetting::onAddBoat()
{
    // select free index
    QVector<bool> indexfree(256, true);
    int index = 0;
    for(int i = 0; i< boatManager->size(); i++){
        indexfree[boatManager->getBoatbyIndex(i)->ID()] = false;
    }
    for(int i =0; i<256; i++){
        if(indexfree[i] == true){
            index = i;

            break;
        }
    }

    QString newboatname = "unknown";
    boatManager->addBoat(index, newboatname, "", "");

    ui->BoatcomboBox->addItem(newboatname,newboatname);
    ui->BoatcomboBox->setCurrentIndex(ui->BoatcomboBox->count()-1);
    ui->BoatlineEdit->setText(newboatname);
    ui->PIPlineEdit->setText("");
    ui->SIPlineEdit->setText("");

}

void BoatSetting::onDeleteBoat()
{
    int index = ui->BoatcomboBox->currentIndex();
    qDebug()<<"BoatSetting::delete: " <<index;

    // double check dialog
    QMessageBox msgBox;
    msgBox.setText(QString("Are you sure you want to delete ")+ui->BoatcomboBox->currentText());
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret != QMessageBox::Ok){
        return;
    }


    boatManager->deleteBoat(index);

    ui->BoatcomboBox->removeItem(index);

    if(ui->BoatcomboBox->count() == 0){
        ui->infoBox->setVisible(false);
    }

}

void BoatSetting::onBoatSelected(int index)
{
    if(index>-1){
        ui->BoatlineEdit->setText(ui->BoatcomboBox->currentText());
        ui->PIPlineEdit->setText(boatManager->getBoatbyIndex(index)->PIP());
        ui->SIPlineEdit->setText(boatManager->getBoatbyIndex(index)->SIP());
    }
    ui->infoBox->setVisible(true);
    peripheralItemModel->clear();
    peripheralItemModel = new QStandardItemModel();
    QStringList label3 = {"ID", QStringLiteral("USB裝置"), QStringLiteral("類別"),QStringLiteral("狀態")};
    peripheralItemModel->setHorizontalHeaderLabels(label3);
    ui->PeriTableView->setModel(peripheralItemModel);
    ui->PeriTableView->setColumnWidth(0,50);
    ui->PeriTableView->setColumnWidth(2,100);
    ui->PeriTableView->setColumnWidth(3,50);
    ui->PeriTableView->setStyleSheet("QTableView::indicator:checked { background-color: #55ee55;} QTableView::indicator:unchecked { background-color: #ee5555;}");
    ui->PeriTableView->verticalHeader()->setVisible(false);

/*

    if(initialized){
        settings->beginGroup(QString("%1").arg(config));
        int size = settings->beginReadArray(QString("boat/%1/board").arg(index+1));

        if(size!=0){
            for(int i = 0; i < size; i++){

                settings->setArrayIndex(i);

                int ID = settings->value("ID").toInt();
                QString boardName = settings->value("boardName").toString();
                QString boardType = settings->value("boardType").toString();
                if(boardType == QString("FTDI")){
                    boardType = QString("RS485_hub");
                }else if(boardType == QString("Silicon")){
                    boardType = QString("Gyro");
                }
                addBoard(ID, boardName, boardType, false);
            }
        }


        settings->endArray();
        settings->endGroup();
    }

*/
}

void BoatSetting::onBoatDoubleClicked(QModelIndex index)
{
    ui->BoatlineEdit->setText(boatManager->getBoatbyIndex(index.row())->name());
    ui->PIPlineEdit->setText(boatManager->getBoatbyIndex(index.row())->PIP());
    ui->SIPlineEdit->setText(boatManager->getBoatbyIndex(index.row())->SIP());
    ui->BoatcomboBox->setCurrentIndex(index.row());
}

void BoatSetting::onChangePIP()
{
    BoatItem* boat = boatManager->getBoatbyIndex(ui->BoatcomboBox->currentIndex());
    boat->setPIP(ui->PIPlineEdit->text());
}

void BoatSetting::onChangeSIP()
{
    BoatItem* boat = boatManager->getBoatbyIndex(ui->BoatcomboBox->currentIndex());
    boat->setSIP(ui->SIPlineEdit->text());
}

void BoatSetting::onAddDeviceButtonClicked()
{
    /*
    AddDeviceDialog* dialog = new AddDeviceDialog(this);
    dialog->setPeripheralModel(peripheralItemModel, boatManager->getBoatbyIndex(ui->BoatcomboBox->currentIndex()));
    int ret = dialog->exec();
    if(ret==QDialog::Accepted){
        Device newdev = dialog->getDevice();
        currentBoat()->peripherals = dialog->getPeripherals();
        currentBoat()->devices.append(newdev);
        QString cmd = "ma";
        cmd += QString::number(newdev.ID);
        cmd+= ",";
        for(int i = 0; i< newdev.pinUIDList.size(); i++){
            cmd += QString::number(newdev.pinUIDList[i]);
            if(i != newdev.pinUIDList.size()-1){
                cmd += " ";
            }else{
                cmd+= ",";
            }

        }
        cmd += QString::number(newdev.type);
        cmd += ",";
        if(newdev.settings == QString()){
            cmd += "n";
        }
        qDebug()<<cmd;

        if(dialog->getDevice().pinUIDList.size() != 0){
            upDateDeviceTableView();

        }

    }
    */
}

void BoatSetting::setPrimaryPriority()
{
    emit connectionTypeChanged(0);
}

void BoatSetting::setSecondaryPriority()
{
    emit connectionTypeChanged(1);
}
