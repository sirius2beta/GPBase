# Build environment:
#  = Windows 11
#  = Qt 5.15.2
#  = mingw64
#  = GStreamer-1.0-mingw64 1.20.3

QT       += core gui network multimedia quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
#CONFIG += with_cuda

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += DEBUG_MODE

INCLUDEPATH += \
    dnapi/ \

SOURCES += \
    adddevicedialog.cpp \
    dnapi/boatitem.cpp \
    dnapi/boatmanager.cpp \
    dnapi/configmanager.cpp \
    dnapi/dnmetadata.cpp \
    dnapi/dnvalue.cpp \
    dnapi/sensoritem.cpp \
    dnapi/sensormanager.cpp \
    boatsetting.cpp \
    comboboxdelegate.cpp \
    configdialog.cpp \
    createwindowdialog.cpp \
    device.cpp \
    dnapi/videomanager.cpp \
    dnapi/gpbcore.cpp \
    dnapi/heartbeat.cpp \
    main.cpp \
    mainwindow.cpp \
    dnapi/networkmanager.cpp \
    sensorlabel.cpp \
    sensorwidget.cpp \
    setpindialog.cpp \
    dnapi/videoitem.cpp \
    addsensordialog.cpp \
    videosettingsdialog.cpp \
    videowindow.cpp \
    dnapi/QmlObjectListModel.cpp\

HEADERS += \
    QTypes.h \
    adddevicedialog.h \
    dnapi/boatitem.h \
    dnapi/boatmanager.h \
    dnapi/configmanager.h \
    dnapi/dnmetadata.h \
    dnapi/dnvalue.h \
    dnapi/sensoritem.h \
    dnapi/sensormanager.h \
    boatsetting.h \
    comboboxdelegate.h \
    configdialog.h \
    createwindowdialog.h \
    device.h \
    dnapi/videomanager.h \
    dnapi/gpbcore.h \
    dnapi/heartbeat.h \
    mainwindow.h \
    dnapi/networkmanager.h \
    sensorlabel.h \
    sensorwidget.h \
    setpindialog.h \
    dnapi/videoitem.h \
    addsensordialog.h \
    videosettingsdialog.h \
    videowindow.h \
    dnapi/QmlObjectListModel.h\

FORMS += \
    ui/adddevicedialog.ui \
    ui/addsensordialog.ui \
    ui/boatsetting.ui \
    ui/configdialog.ui \
    ui/createwindowdialog.ui \
    ui/mainwindow.ui \
    ui/networksettingscontrol.ui \
    ui/sensorlabel.ui \
    ui/sensorwidget.ui \
    ui/setpindialog.ui \
    ui/videosettingsdialog.ui \
    ui/videowindow.ui



win32{
    INCLUDEPATH += "C:\gstreamer_MSVC\1.0\msvc_x86_64\include"
    INCLUDEPATH += "C:\gstreamer_MSVC\1.0\msvc_x86_64\include\gstreamer-1.0"
    INCLUDEPATH += "C:\gstreamer_MSVC\1.0\msvc_x86_64\include\glib-2.0"
    INCLUDEPATH += "C:\gstreamer_MSVC\1.0\msvc_x86_64\lib\glib-2.0\include"
    INCLUDEPATH += "C:\gstreamer_MSVC\1.0\msvc_x86_64\lib\gstreamer-1.0\include"


    CONFIG += link_pkgconfig
    PKGCONFIG += gstreamer-1.0

    LIBS += -LC:\gstreamer_MSVC\1.0\msvc_x86_64/lib/ -lgstvideo-1.0
    LIBS += -LC:/Qt/5.15.2/msvc2019_64/lib/ -lQt5Mqtt
}

unix{
        macx {
            CONFIG += link_pkgconfig
            PKGCONFIG += gstreamer-1.0
            INCLUDEPATH += /Library/Frameworks/GStreamer.framework/Versions/1.0/Headers
            LIBS += -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 -lgstvideo-1.0
        }
         else {
                # Linux definitions go here ...
        }
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

DISTFILES += \
    logo.rc
RC_FILE += \
    logo.rc
