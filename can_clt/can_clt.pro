#-------------------------------------------------
#
# Project created by QtCreator 2017-03-09T11:44:09
#      Add in Run section for copy om mount disk mnt_imx
#      cp
#      %{CurrentProject:NativePath}/../Run_project_release/%{CurrentProject:Name} /home/dnadev/mnt_imx
#
#-------------------------------------------------

QT       += core
QT       += serialport
VERSION = 1.00.00

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

include (../config.pro)

TEMPLATE = app
CONFIG += console app
CONFIG -= app_bundle
CONFIG += exception
MOC_DIR = ../../moc
OBJECTS_DIR = ../../obj
QT+= widgets

TARGET = can_clt
target.files = TARGET
target.path = /home/user/prog/dev_srv
INSTALLS += target
export(INSTALLS)

SOURCES += main.cpp can_commsock.c canclient.cpp
HEADERS += canclient.h can_ids.h can_commsock.h


# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_CFLAGS_RELEASE -= -O
QMAKE_CFLAGS_RELEASE -= -O1
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE -= -O3
#QMAKE_CXXFLAGS +=O0

DESTDIR = ../../$${CURRENT_BUILD}
