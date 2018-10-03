#-------------------------------------------------
#
# Project created by QtCreator 2018-08-21T09:50:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Map2d
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/local/include

LIBS += /usr/local/lib/libopencv_core.so \
        /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_imgproc.so \
        /usr/local/lib/libopencv_imgcodecs.so \
        /usr/local/lib/libopencv_videoio.so \
        /usr/local/lib/libopencv_xfeatures2d.so \
        /usr/local/lib/libopencv_features2d.so \
        /usr/local/lib/libopencv_xobjdetect.so \
        /usr/local/lib/libopencv_objdetect.so \
        /usr/local/lib/libopencv_flann.so \
        /usr/local/lib/libopencv_calib3d.so \
        /home/yfji/Workspace/OpenCV/ImageStitch/lib/libgms.so

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    imagestitcher.cpp \
    feature.cpp \
    orb_feature.cpp \
    mapmanager.cpp \
    baseimageinput.cpp \
    videoimageinput.cpp \
    cameraimageinput.cpp \
    filelistimageinput.cpp \
    ipcamimageinput.cpp \
    maplabel.cpp \
    cameracalibration.cpp \
    gpsclient.cpp \
    mission.cpp \
    salient.cpp

HEADERS += \
        mainwindow.h \
    imagestitcher.h \
    feature.h \
    orb_feature.h \
    gms_matcher.h \
    Header.h \
    mapmanager.h \
    baseimageinput.h \
    videoimageinput.h \
    cameraimageinput.h \
    filelistimageinput.h \
    ipcamimageinput.h \
    maplabel.h \
    cameracalibration.h \
    gpsclient.h \
    mission.h \
    salient.h

FORMS += \
        mainwindow.ui
