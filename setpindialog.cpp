#include "setpindialog.h"
#include "qmessagebox.h"
#include "ui_setpindialog.h"
#include <QDebug>

SetPinDialog::SetPinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetPinDialog),
    itemReady(false),
    devType(0),
    devID(0),
    maxNum(0)
{
    ui->setupUi(this);

    pinItemModel = new QStandardItemModel(this);



    ArduinoPinMap = QList<QString> {"0", "0", "03", "03", "", "", "0", "02", "01", "02",
                                    "012", "0", "01", "012", "012", "02", "01", "02", "", "",
                                    "01", "01", "01", "01", "01", "01", "", "", "", "",
                                    "", "", "0", "0"};
    ArduinoPinInfo = QList<QString> {"   D16/COPI", "   D17", "   D1/TX", "   D0/RX", "RESET", "GND", "   D2", "~D3", "   D4/A6", "~D5",
                                    "~D6/A7", "   D7", "   D8/A8", "~D9/A9", "~D10/A10", "~D11", "   D12/A11", "~D13", "+3V", "AREF",
                                    "   D18/A0", "   D19/A1", "   D20/A2", "   D21/A3", "   D22/A4", "   D23/A5", "NC", "NC", "+5V", "RESET",
                                    "GND", "VIN", "D14", "D15"};
    IOMap = QList<QString> {"01", "1", "0", "01"};


    ui->pinTableView->verticalHeader()->setDefaultSectionSize(10);
    connect(ui->boardTableView,&QTableView::clicked, this, &SetPinDialog::onBoardSelected);
    connect(pinItemModel, &QStandardItemModel::itemChanged, this, &SetPinDialog::onDeviceItemChanged);
}

SetPinDialog::~SetPinDialog()
{
    delete ui;
}

void SetPinDialog::setPeripheralModel(QStandardItemModel* pm, BoatItem* boat){
    thisBoat = boat;
    this->peripheralItemModel = pm;
    QStringList label = {"ID", QStringLiteral("裝置名稱"), QStringLiteral("USB裝置"), QStringLiteral("狀態")};
    //peripheralItemModel->setHorizontalHeaderLabels(label);
    ui->boardTableView->setModel(peripheralItemModel);
    ui->boardTableView->setColumnWidth(0,50);

    ui->boardTableView->setColumnWidth(2,100);
    ui->boardTableView->setColumnWidth(3,50);
    ui->boardTableView->verticalHeader()->setVisible(false);
    PeripheralCache = boat->peripherals;

}

void SetPinDialog::setType(int type)
{
    devType = type;
}

void SetPinDialog::setDev(const Device& Dev)
{
    newDev = Dev;
}

void SetPinDialog::setPeripherals(const QList<Peripheral>& peripherals)
{
    PeripheralCache = peripherals;
}

void SetPinDialog::setMaxNum(int num)
{
    maxNum = num;
}

void SetPinDialog::setIndex(int i)
{
    index = i;
}

int SetPinDialog::getIndex()
{
    return index;
}

QList<int> SetPinDialog::getpinCurrentSelected()
{
    return pinCurrentSelected;
}

QList<Peripheral> SetPinDialog::getPeripherals()
{
    return PeripheralCache;
}

const Device& SetPinDialog::getDevice(){
    return newDev;
}

void SetPinDialog::onBoardSelected(const QModelIndex &index){
    itemReady = false;
    currentRow = index.row();

    if(peripheralItemModel->item(index.row(),1)->text() == "Arduino LLC"){
        pinItemModel->clear();
        QStringList label = {QStringLiteral("啟用"), "ID", "Pin",QStringLiteral("裝置名稱")};
        pinItemModel->setHorizontalHeaderLabels(label);
        ui->pinTableView->setModel(pinItemModel);
        ui->pinTableView->setColumnWidth(0,40);
        ui->pinTableView->setColumnWidth(1,40);
        ui->pinTableView->setColumnWidth(2,120);

        ui->pinTableView->verticalHeader()->setVisible(false);
        ui->pinTableView->setStyleSheet("QTableView::indicator:checked { background-color: #55ee55;} QTableView::indicator:unchecked { background-color: #ee5555;}");

        for(int i = 0; i<34; i++){
            QStandardItem* item1 = new QStandardItem();
            item1->setCheckable(true);
            item1->setCheckState(Qt::Unchecked);
            QStandardItem* item2 = new QStandardItem(QString::number(i+1));
            QStandardItem* item3 = new QStandardItem("unknown");
            QStandardItem* item4 = new QStandardItem(ArduinoPinInfo[i]);

            pinItemModel->setItem(i,0,item1);
            pinItemModel->setItem(i,1,item2);
            pinItemModel->setItem(i,2,item4);
            pinItemModel->setItem(i,3,item3);
            ui->pinTableView->setRowHeight(i,1);

        }
        int currentBoardID = PeripheralCache[index.row()].ID;
        for(int i = 0; i<PeripheralCache[index.row()].pinSelected.size(); i++){
                bool cont = false;
                for(int j = 0; j<pinItemModel->rowCount(); j++){
                   for(int k = 0; k<pinCurrentSelected.size(); k++){
                       int boardID = pinCurrentSelected[k]/256;
                       int DevID = pinCurrentSelected[k]%256;
                       if(boardID == currentBoardID){
                           if(pinItemModel->item(j, 1)->text().toInt() == DevID){
                               pinItemModel->item(j,0)->setCheckState(Qt::Checked);
                               cont = true;
                               break;
                           }
                       }

                   }
                   if(!cont){
                       //其他dev選擇的pin -> deactivate
                        if(pinItemModel->item(j, 1)->text().toInt() == PeripheralCache[index.row()].pinSelected[i]){
                            pinItemModel->setItem(j,0,0);

                        }
                   }
                }

        }
    }else{
        pinItemModel->clear();
    }
    itemReady = true;
}

void SetPinDialog::onDeviceItemChanged(QStandardItem *item)
{
    if(itemReady){
        if(item->column() == 0){ // Activate欄，切換開啟關閉
           if(item->checkState()){ // 開啟時
               if(maxNum<=pinCurrentSelected.size()){ //超過最多能選擇個數
                   QMessageBox* msgbox = new QMessageBox(this);
                   msgbox->setText("exceed maximum selected pin number!");
                   msgbox->exec();
                   item->setCheckState(Qt::Unchecked);
                   return;
               }
               //新增至Device pinlist中
               int pinID = pinItemModel->item(item->row(),1)->text().toInt();
               int uid = PeripheralCache[currentRow].ID*256+pinID;
               newDev.pinUIDList.append(uid);
               pinCurrentSelected.append(uid);
               /*
               qDebug()<<"ID:"<<newDev.ID;
               qDebug()<<"uid:"<<uid;
               qDebug()<<"name:"<<newDev.deviceName;
               qDebug()<<"type:"<<newDev.type;
               */
               //新增至Peripheral pinselected中
               PeripheralCache[currentRow].pinSelected.append(pinID);
           }else{
               int pinID = pinItemModel->item(item->row(),1)->text().toInt();
               int uid = PeripheralCache[currentRow].ID*256+pinID;
               for(int i = 0; i<newDev.pinUIDList.size(); i++){
                   if(newDev.pinUIDList[i] == uid){
                       newDev.pinUIDList.removeAt(i);
                   }
               }
               for(int i = 0; i<PeripheralCache[currentRow].pinSelected.size(); i++){
                   if(PeripheralCache[currentRow].pinSelected[i] == pinID){
                       PeripheralCache[currentRow].pinSelected.removeAt(i);
                   }
               }
               for(int i = 0; i<pinCurrentSelected.size(); i++){
                   if(pinCurrentSelected[i] == uid){
                       pinCurrentSelected.removeAt(i);
                   }
               }


           }

        }

    }

}
