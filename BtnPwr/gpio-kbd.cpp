#include "gpio-kbd.h"

/*!
 * \file gpio-kbd.cpp
 * \brief Реализация методов класса TkbdGpio.
 *
 * Реализация методов класса TkbdGpio.
 * Поддерживается GPIO клавиатура для пяти кнопок платы IMX6_1.
 * Возвращаются сигнал keyPressed(int) с кодом пяти клавиш:
 * 0 - J3A.3
 * 1 - J3B.2
 * 2 - J3B.3
 * 3 - J3B.4
 * 4 - J3B.5
 * Активный уровень низкий. Сигнал вырабатывается на отпускании
 * клавиши. При длительном нажатии клавиши (более 5 с) в
 * бите 7 устанавливается единица.
 * \sa keyPressed(int)
 */


/*!
 * Конструктор класса. После создания класса вызвать функцию
 * getError(), для проверки на наличие ошибок. При отсутствии
 * ошибок возвращает 0.
 * \sa getError()
 */
TkbdGpio::TkbdGpio()
{
  qDebug()<<  "TkbdGpio object constructor start.";
  abort=false;
  error=0;
  kev_ev=0;
  fd_ev=0;
  error=initalKBD();
  qDebug()<<  "TkbdGpio object constructor finish.";
}

/*!
 * Деструктор класса.
 */
TkbdGpio::~TkbdGpio()
{
  qDebug()<<  "TkbdGpio object destructor start.";
  abort=true;
  releaseKBD();
  terminate();
  wait(1000);
  qDebug()<< "TkbdGpio object destructor finish.";
}

/****************************************************************
 * Private functions
 *
 ****************************************************************/
/*!
 * Инициализация аппаратуры и создания ресурсов для работы с GPIO
 * клавиатурой.
 *
 * \return Код возврата: 0 - нет ошибок, 1 - ошибка создания
 * обработчика событий, 2 - ошибка открытия дескриптора входных событий,
 * 3 - ошибка конфигурирования обработчика событий.
 */
int TkbdGpio::initalKBD(void)
{
  kev_ev=epoll_create(5);
  if (kev_ev<0) {
    perror("epoll_create");
    return(1);
  }
  fd_ev=open(INPUT_EVENT,O_RDWR);
  if (fd_ev<0) {
    perror("open input/event");
    close(fd_ev);
    return(2);
  }
  ev_ev.data.fd = fd_ev; /* return the fd to use later */
  ev_ev.events = EPOLLIN|EPOLLET;
  int ret;
  ret = epoll_ctl (kev_ev, EPOLL_CTL_ADD, fd_ev, &ev_ev);
  if(ret) return 3;
  return(0);
}

/*!
 * Освобождение ресурсов для работы с GPIO
 * клавиатурой. Вызывается деструктором.
 */
void TkbdGpio::releaseKBD(void)
{
  if (kev_ev>0) {
    close(kev_ev);
  }
  if (fd_ev>0) {
    close(fd_ev);
  }
  fd_ev=0;
  kev_ev=0;
}
/*!
 * Обработка нажатий клавиатуры и отправка сигнала  keyPressed(int)
 * с кодом пяти клавиш:
 * 0 - J3A.3
 * 1 - J3B.2
 * 2 - J3B.3
 * 3 - J3B.4
 * 4 - J3B.5
 * Активный уровень низкий. Сигнал вырабатывается на отпускании
 * клавиши. При длительном нажатии клавиши (более 5 с) в
 * бите 7 устанавливается единица.
 */
void TkbdGpio::run()
{
  int len;
  int time_press=0,longPress=0,tspend;
  while(!abort) {
    len=read(fd_ev,&keydat,sizeof(keydat));
    if(len!=sizeof(keydat)) continue;
    if (keydat.type!=EV_KEY) continue;
    printf("Evtype %d, key %d. val%d, (%d)\n",keydat.type,keydat.code,keydat.value,len);
    if (keydat.value==1) { // press button
      time_press=keydat.time.tv_sec;
      continue; // wait for button release
    }
    if (keydat.value==0) { // release button
      tspend=keydat.time.tv_sec-time_press;
      if (tspend>PRESS_TIMEOUT2) {
        longPress=128;
      }
      else if((tspend>=PRESS_TIMEOUT1)&&(tspend<=PRESS_TIMEOUT2)){
        longPress=64;
      }
      else {
        longPress=0;
      }
    }
    switch(keydat.code){
    case KEY_F5:{
      qDebug()<<"Time"<<tspend ;
      emit keyPressed(longPress);
      break;
    }
    case KEY_F1:{
      emit keyPressed(longPress|1);
      break;
    }
    case KEY_F2:{
      emit keyPressed(longPress|2);
      break;
    }
    case KEY_F3:{
      emit keyPressed(longPress|3);
      break;
    }
    case KEY_F4:{
      emit keyPressed(longPress|4);
      break;
    }
    default:
      break;
    }
    longPress=0;
  }
}

