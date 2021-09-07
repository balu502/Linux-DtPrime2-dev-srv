#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include <QtCore/QCoreApplication>
#include <QDebug>

#include "canclient.h"



int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  TCANChannels *ch;
  int opt,cannode=2,event=0;
  char cmd[80],reply[80];
  char *pcmdToExec=NULL;
  ch=NULL;
  do{
    opt=getopt(argc,argv,"n:c:e:");
    if(opt!=-1)
      switch(opt)  {
      case 'n':
        cannode=strtol(optarg,NULL,0);
      break;
      case 'c': // run command
        pcmdToExec=optarg;
        strcpy(cmd,pcmdToExec);
      break;
       case 'e':
         event=strtol(optarg,NULL,0);
       break;
    }
  }
  while(opt!=-1);

  ch=new TCANChannels(cannode);
  if(!ch) printf("CAN channel create error\n");

  if(event) {
    CAN_SendEvent(event);
    delete ch;
    exit(0);
  }

  if (!ch->Open()) {
    printf("CAN channel open error, node %d\n",cannode);
    exit(-1);
  }
  if(ch->Cmd(cmd,reply,sizeof(reply))) {
    printf("%s\n",reply);
  }
  else {
    printf("error in CAN transaction, node %d\n",cannode);
    ch->Reset();
    exit(-1);
  }
  ch->Close();
  delete ch;
  exit(0);

}
