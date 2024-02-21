# Build environment:
#  = Windows 11
#  = Qt 5.15.2
#  = mingw64
#  = GStreamer-1.0-mingw64 1.20.3

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
#CONFIG += with_cuda

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += DEBUG_MODE
SOURCES += \
    adddevicedialog.cpp \
    boatitem.cpp \
    boatmanager.cpp \
    boatsetting.cpp \
    comboboxdelegate.cpp \
    configdialog.cpp \
    createwindowdialog.cpp \
    device.cpp \
    gpbcore.cpp \
    heartbeat.cpp \
    main.cpp \
    mainwindow.cpp \
    networkmanager.cpp \
    sensorlabel.cpp \
    sensorwidget.cpp \
    setpindialog.cpp \
    videosettingsdialog.cpp \
    videowindow.cpp \
    #yolov5.cpp

HEADERS += \
    QTypes.h \
    adddevicedialog.h \
    boatitem.h \
    boatmanager.h \
    boatsetting.h \
    comboboxdelegate.h \
    configdialog.h \
    createwindowdialog.h \
    device.h \
    gpbcore.h \
    heartbeat.h \
    mainwindow.h \
    networkmanager.h \
    sensorlabel.h \
    sensorwidget.h \
    setpindialog.h \
    videosettingsdialog.h \
    videowindow.h \
    #yolov5.h

FORMS += \
    ui\adddevicedialog.ui \
    ui\boatsetting.ui \
    ui\configdialog.ui \
    ui\createwindowdialog.ui \
    ui\mainwindow.ui \
    ui\networksettingscontrol.ui \
    ui\sensorlabel.ui \
    ui\sensorwidget.ui \
    ui\setpindialog.ui \
    ui\videosettingsdialog.ui \
    ui\videowindow.ui



INCLUDEPATH += "C:\gstreamer\1.0\msvc_x86_64\include"
INCLUDEPATH += "C:\gstreamer\1.0\msvc_x86_64\include\gstreamer-1.0"
INCLUDEPATH += "C:\gstreamer\1.0\msvc_x86_64\include\glib-2.0"
INCLUDEPATH += "C:\gstreamer\1.0\msvc_x86_64\lib\glib-2.0\include"
INCLUDEPATH += "C:\gstreamer\1.0\msvc_x86_64\lib\gstreamer-1.0\include"


CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0

LIBS += -LC:\gstreamer\1.0\msvc_x86_64/lib/ -lgstvideo-1.0
LIBS += -LC:/Qt/5.15.2/msvc2019_64/lib/ -lQt5Mqtt
with_cuda{
    #INCLUDEPATH += "C:\opencv-4.6\build_with_cuda\install\include"
    #LIBS += -LC:/opencv-4.6/build_with_cuda/install/x64/lib\ -lopencv_world460
} else{
    #INCLUDEPATH += "C:\opencv_MSVC_no_cuda\include"
    #LIBS += -LC:/opencv_MSVC_no_cuda/x64/lib\ -lopencv_world460
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
