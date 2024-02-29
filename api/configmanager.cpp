#include "configmanager.h"
#include <QFile>
#include <QDebug>
ConfigManager::ConfigManager(QObject *parent)
    : QObject{parent}
{
    QFile file("/Users/sirius/test.xml");
    reader.setDevice(&file);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"file opened";

        if (reader.readNextStartElement()) {
            if (reader.name() == "marinelink"){
                while(reader.readNextStartElement()){
                    if(reader.name() == "enum"){
                        if(reader.attributes().value("name") == "SENSOR_TYPE"){

                            readSensorTypes();
                        }else if(reader.attributes().value("name") == "MESSAGE_TYPE"){
                            reader.skipCurrentElement();

                        }

                    }
                    else
                        reader.skipCurrentElement();
                }
            }
            else
                reader.raiseError(QObject::tr("Incorrect file"));
        }




    }
    file.close();
    qDebug()<<"file closed";
}

void ConfigManager::readSensorTypes()
{
    while(reader.readNextStartElement()){

        if(reader.name() == "entry"){
            qDebug()<<reader.attributes().value("name");

            // readElementText at last to prevent breaking loop
            QString s = reader.readElementText();
        }
    }
}
