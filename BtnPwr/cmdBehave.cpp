#include "cmdBehave.h"
#include "btnBehave.h"
/*!
 * \file cmdBehave.cpp
 * \brief Реализация методов класса TcmdBehav.
 */

extern TBtnBehav *device;
/*!
 * Конструктор класса.
 */
TcmdBehav::TcmdBehav()
{
  qDebug()<<  "TcmdBehav object constructor start.";
  abort=false;
  qDebug()<<  "TcmdBehav object constructor finish.";
  start(QThread::NormalPriority);
}

/*!
 * Деструктор класса.
 */
TcmdBehav::~TcmdBehav()
{
  qDebug()<<  "TcmdBehav object destructor start.";
  abort=true;
  wait(3000);
  qDebug()<< "TcmdBehav object destructor finish.";
}

/****************************************************************
 * Protected functions
 *
 ****************************************************************/

/*!

 */
void TcmdBehav::run()
{
  QFile fctrl("/mnt/ramdisk/ctrl.req");
  while(!abort) {
    msleep(2000);
    if(fctrl.exists()){
      fctrl.open(QIODevice::ReadOnly | QIODevice::Text);
      QByteArray line = fctrl.readLine();
      printf("get command %s\n",line.data());
      fctrl.close();
      fctrl.remove();
      if(!strncmp(line.data(),"RSTP",4)) {
        if(device->getPwrBtnSts()){
          printf("exec command %s\n",line.data());
          QProcess::execute("/etc/init.d/gui_dt.sh",QStringList()<<"restart");
        }
      }
      if(!strncmp(line.data(),"RSTS",4)) {
        printf("exec command %s\n",line.data());
        QProcess::execute("reboot");
      }
      if(!strncmp(line.data(),"SHTD",4)) {
        printf("exec command %s\n",line.data());
        //QProcess::execute("/etc/init.d/userprog.sh",QStringList()<<"stop");
        QProcess::execute("shutdown -hP 0");
      }
      if(!strncmp(line.data(),"SWUP",4)) {
        printf("exec command %s\n",line.data());
        QFile fupdate("/mnt/ramdisk/update");
        fupdate.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&fupdate);
        while (!in.atEnd()) {
          QString line = in.readLine();
          if(line.size()) {
            fupdate.close();
            QProcess::execute("/home/user/prog/update.sh",QStringList()<<line);
            break;
          }
          fupdate.close();
        }
      }
      if(!strncmp(line.data(),"NETW",4)) {
        printf("exec command %s\n",line.data());
        QFile inFile("/etc/network/interfaces"),outFile("/mnt/ramdisk/tmp"),dataFile("/mnt/ramdisk/interfaces");
        if(!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) continue;
        if(!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) continue ;
        if(!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) continue ;
        QTextStream in(&inFile),out(&outFile),inData(&dataFile);
        QStringList dataList;
        dataList.clear();
        while(!inData.atEnd()){
          dataList.append(inData.readLine());
        }
        dataFile.close();
        while (!in.atEnd()) {
          QString line=in.readLine().simplified();
          if(line.data()[0]!='#') {
            for(int i=0;i<dataList.size();i++){
              QString tmp= dataList.at(i);
              QStringList tmpList=tmp.simplified().split(' ');
              if(!tmpList.empty()){
                if(line.contains(tmpList.at(0))) {
                  line=tmp;
                }
              }
            }
          }
          out<<line<<'\n';
        }
        inFile.close();
        outFile.close();
        QProcess::execute("mv /mnt/ramdisk/tmp /etc/network/interfaces");
      }
    }
  }
}

