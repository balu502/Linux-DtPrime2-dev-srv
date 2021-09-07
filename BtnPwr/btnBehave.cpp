
#include "btnBehave.h"
#include "canclient.h"
#include "can_ids.h"


//-----------------------------------------------------------------------------
//--- Constructor
//-----------------------------------------------------------------------------
TBtnBehav::TBtnBehav()
{
  qDebug()<<"TBtnBehav constructor start.";
 // syslog(LOG_INFO,"TBtnBehav object initialisation start.");
  kbdGpio=0;

 // init GPIO pins
  kbdGpio=new TkbdGpio();
  Temp_uC=0;
  Temp_uC=new TCANChannels(UCTEMP_CANID);
  if(kbdGpio){
    if(kbdGpio->getError()) {
      qDebug()<<"KBD open error";
      syslog( LOG_ERR,"Can't use GPIO buttons.");
    }
    else {
      connect(kbdGpio,SIGNAL(keyPressed(int)),this,SLOT(slotKbdPressed(int)));
      kbdGpio->start(QThread::NormalPriority);
    }
  }
  btn_state=1;
  sleep=0;
  qDebug()<<"TBtnBehav constructor finish.";
}

//-----------------------------------------------------------------------------
//--- Destructor
//-----------------------------------------------------------------------------
TBtnBehav::~TBtnBehav()
{
  qDebug()<<"TBtnBehav destructor start.";
  if(kbdGpio) { delete kbdGpio; kbdGpio=0; }
  qDebug()<<"TBtnBehav destructor finish.";
}


//-----------------------------------------------------------------------------
//--- timer timeout event. On this event can get data from device
//-----------------------------------------------------------------------------
void TBtnBehav::slotKbdPressed(int code)
{
  //static int btn_state=1;

  int run=0;
  while(1){
    if(code==0) {
      qDebug()<<"Light sleep or wake up";
      if((btn_state==0)&&(sleep==1)){  //turn on power
        qDebug()<<"Wake up from short key press";
        QProcess::execute("init 5");
        btn_state=1;
        sleep=0;
      }
    }
    if(code==64){ // press Powwer button
      if(btn_state==0){  //turn on power
        qDebug()<<"Wake up";
        QProcess::execute("init 5");
        btn_state=1;
        sleep=0;
      }
      else {  // turn of power
        if(USBCy_RW("RSTS",answer,Temp_uC)) { // check RUN program on t-controller
          QTextStream(&answer) >> run;
        }
        qDebug()<<"Sleep";
        if(run&1) break; // RUN only
        sleep=1;
        QProcess::execute("init 2");
        btn_state=0;
      }
    }
    if(code==128){ // press long button
      qDebug()<<"Shutdown";
      QProcess::execute("shutdown -hP 0");
    }
    break;
  }
}

//-----------------------------------------------------------------------------
//--- Read/write from CAN
//-----------------------------------------------------------------------------
bool TBtnBehav::USBCy_RW(QString cmd, QString &answer, TCANChannels *uC)
{
  unsigned int i;
  static  char buf[64];
  if (!uC) return false;
  QByteArray str;
  bool sts = false;
  int n=5;
  while((!sts)&&((n--)!=0)){
    if(uC->Open()) {
      for(i=0; i<sizeof(buf); i++) buf[i] = '\0';
      str = cmd.toLatin1();
      if(uC->Cmd((char*)str.constData(), buf, sizeof(buf))) {
        answer = QString::fromLatin1((char*)buf,-1);                                // ok
        if(answer.count() >= 3 && answer[2] == '>') { answer.remove(0,3); sts = true; if(answer[0]=='?') {sts=false; break;}}            //
        answer = answer.simplified();   // whitespace removed: '\t','\n','\v','\f','\r',and ' '
      }
    }
    if(!uC->Close())            // Close uC
    {
      uC->Reset();
      uC->Close();
    }
  }
  if(!sts)
  {
    answer="error";
  }
  return(sts);
}
