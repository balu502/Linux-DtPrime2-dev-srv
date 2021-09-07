#-------------------------------------------------
#
# Project created by QtCreator 2016-02-04T15:59:10
#
#      Add in Run section for copy om mount disk mnt_imx
#      cp
#      %{CurrentProject:NativePath}/../Run_project_release/%{CurrentProject:Name} /home/dnadev/mnt_imx
#      or %{CurrentProject:NativePath}/../../Run_project_release/srv_dt /home/dnadev/mnt_imx/devsrv
#      upload 172.18.0.10 from sftp
#      TARGET = srv_dt
#      target.files = TARGET
#      target.path = /home/root/devsrv
#      INSTALLS += target
# was old linguist %{CurrentProject:QT_INSTALL_BINS}/lupdate
#-------------------------------------------------

QT       += core
QT       -= gui

VERSION = 2.01.00

#version 1.00.00 begin
#version 2.00.00 Connect wm_dt  and powerBtn
#version 2.01.00 New key processing 0-2sec light sleep 2-5sec sleep 5 ... shutdown


DEFINES += APP_VERSION=\\\"$$VERSION\\\"
MOC_DIR = ../../moc
OBJECTS_DIR = ../../obj
include (../config.pro)

TARGET = btnpwr_dt
target.files = TARGET
target.path = /home/user/prog/dev_srv
INSTALLS += target
export(INSTALLS)

TEMPLATE = app

CONFIG += console
CONFIG += app
CONFIG -= app_bundle
CONFIG += exception


SOURCES += \
    main.cpp btnBehave.cpp cmdBehave.cpp canclient.cpp gpio-kbd.cpp can_commsock.c
HEADERS += \
    btnBehave.h cmdBehave.h canclient.h gpio-kbd.h can_commsock.h

DESTDIR = ../../$${CURRENT_BUILD}

# remove possible other optimization flags
#QMAKE_CXXFLAGS_RELEASE -= -O
#QMAKE_CXXFLAGS_RELEASE -= -O1
#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE -= -O3
#QMAKE_CFLAGS_RELEASE -= -O
#QMAKE_CFLAGS_RELEASE -= -O1
#QMAKE_CFLAGS_RELEASE -= -O2
#QMAKE_CFLAGS_RELEASE -= -O3
#QMAKE_CXXFLAGS +=O0


















