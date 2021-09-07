#ifndef BTNBEHAV_H
#define BTNBEHAV_H

#include <QThread>
#include <QDebug>


#include <syslog.h>

#include "canclient.h"
#include "gpio-kbd.h"


class TBtnBehav : public QObject
{
  Q_OBJECT
public:
  TBtnBehav();
  ~TBtnBehav();
public :
  int getPwrBtnSts(void){ return(btn_state); }
private:
  TkbdGpio  *kbdGpio;
  TCANChannels *Temp_uC;
  QString answer;
  int btn_state,sleep;
  bool USBCy_RW(QString cmd, QString &answer, TCANChannels *uC);

private slots:
  void slotKbdPressed(int);
};

#endif // BTNBEHAV_H
