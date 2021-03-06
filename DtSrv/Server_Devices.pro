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
#-------------------------------------------------
#@ dtbehave.cpp remove  507 1816 1837 for test
QT       += core
QT       += network
QT       += serialport
QT       -= gui

VERSION = 2.00.00
COMPATIBLE_VERSION = 7.30
DEFINES += MIN_VERSION=\\\"$$COMPATIBLE_VERSION\\\"

#version 1.00.00 begin
#version 1.00.01 change error processing
#1.00.05 new state machine processing + add Linguist
#1.00.07 change error processing model
#1.00.09 small error was removed
# press cover add, barcode read procedure in close cover procedure add.
#1.00.10 video online read function add
#1.00.11 ARM version with call signal for processing network function from thread
#1.00.12 ARM remove main QThread and direct call of network function
#1.00.13 ARM USB rebuild
#        in can_commsock change function CAN_CloseChan
#        while (!(CANCh[chan].state&CC_RXRPL)) {  was CANCh[i]!
#        CANCh[chan].state=0; // clear "got reply" flag, close channel
#1.01.00 remove widgets from pro file. Replace QApplication on QCoreApplication
#        new error processing remove GLOBAL_ERROR_STATE. And remove INITIAL_STATE. At run going on GETINFO_STATE
#1.01.01 Ver 1.01.00 with remove unuse code comments
#1.01.02 Add USB scanner processing
#1.01.03 Remove hidusb API was problem with libusb
#1.01.04 Solve problems with hidusb API. hidusb API  add again.
#1.01.05 Cosmetical. Remove devname from log
#1.01.06 Add log information in map_InfoData
#        Remove USB segmentation on left flash device. New dstate on meas flat recogniase methode for simple USB.
#1.01.07 Add high tube measure. Add protocol info request in begin of run
#        for simple USB mode
#1.01.08 Errors rename in file progerror.h (_ERR) in enum CodeErrors
#1.01.09 12.03.2020 Send wake up request before run.
#1.01.10 03.06.2021 test version
# DPIC add
#2.00.00 5.08.2021
# New opical uc 

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
MOC_DIR = ../../moc
OBJECTS_DIR = ../../obj
include (../config.pro)

TARGET = srv_dt
target.files = TARGET
target.path = /home/user/prog/dev_srv
INSTALLS += target
export(INSTALLS)

TEMPLATE = app

CONFIG += console
CONFIG += app
CONFIG -= app_bundle
CONFIG += exception

LIBS += -lusb-1.0

SOURCES += main.cpp\
          dtBehav.cpp \
          logobject.cpp \
          can_commsock.c \
          progerror.cpp \
          gpio.cpp \
          fx2usb.cpp \
          canclient.cpp \
          cansrv.cpp \
          serialscanner.cpp \
          usbscanner.cpp \
          hid.c

HEADERS +=dtBehav.h \
          logobject.h \
          progerror.h\
          can_ids.h \
          can_commsock.h \
          digres.h \
          gpio.h \
          request_dev.h \
          fx2usb.h \
          canclient.h \
          cansrv.h \
          serialscanner.h \
          usbscanner.h \
          hidapi.h

DESTDIR = ../../$${CURRENT_BUILD}

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


















