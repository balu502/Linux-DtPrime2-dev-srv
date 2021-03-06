
/*
 * hw.cpp
 *
 */

#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

//#include <QApplication>
#include <QtCore>

/****************************************************************
 * Object constructor
 ****************************************************************/
TGpio::TGpio()
{
  qDebug()<<  "TGpio object constructor start.";  
  fd_d0=0; fd_d1=0;
  abort=false;
  qDebug()<<  "TGpio object constructor finish."; 
}

/****************************************************************
 * destructor
 * Unexport pins and free spi1
 ****************************************************************/
TGpio::~TGpio()
{
  qDebug()<<  "TGpio object destructor start.";
  abort=true;
  condition.wakeOne();
  wait();
  close_gpios();
  pins_unexport();
  qDebug()<< "TGpio object destructor finish.";
}

void TGpio::run()
{
  //CPhase deb=READY;//for debug only
  QEventLoop *loop ;
  loop= new QEventLoop();

//  unsigned int val;
//  getGPIO(ACT,&val);
//  if(val){
//    setGPIO(GPIO2,1); // simple USB mode
//  }
//  else {
    setGPIO(GPIO2,0); // normal USB mode
//  }
    uint val,val1,val2,val3,val4;
    int mfd;
      int kev,kfd,ret; //keys input event epoll dsc
      struct epoll_event event,evs[10];
      kev=epoll_create(5);
       if (kev<0) {
         perror("epoll_create");
         exit(1);
       }
       kfd=open("/dev/input/event1",O_RDWR);
       if (kfd<0) {
         perror("open");
         close(kev);
         exit(1);
       }
       event.data.fd = kfd; /* return the fd to us later */
       event.events = EPOLLIN|EPOLLET;
       ret = epoll_ctl (kev, EPOLL_CTL_ADD, kfd, &event);
       int time_press=0,butt_state=0;
  while(!abort) { // run until destructor not set abort

    //if(polling_gpios(500)==1)
    {
          int nr_events,len;
              struct input_event keydat;
              len=read(kfd,&keydat,sizeof(keydat));

  if (keydat.type!=EV_KEY) continue;
  printf("Evtype %d, key %d. val%d, (%d)\n",keydat.type,keydat.code,keydat.value,len);
  if (keydat.code==KEY_WAKEUP) {
    int ret;
    if (keydat.value==1) { // press button
   time_press=keydat.time.tv_sec;
  qDebug()<<"press";
  continue; // wait for button release

    }
  }
  if (keydat.value==0) {
      if ((keydat.time.tv_sec-time_press)>3) {
          qDebug()<<"REBOOT";
        //ret=system("/sbin/reboot");
        //syslog( LOG_ERR, "system reboot, ret %d",ret);
        //exit(1); // exit to prevent wrong button after press
      }
      else {
        if (butt_state==0) { // if state is off, turn it on
          //ret=system("init 3");
          //syslog( LOG_ERR, "Power ON, init ret %d",ret);
            qDebug()<<"WAKE UP";
          butt_state=1;
        }
        else { // turn off otherwise
         // printf("Power OFF\n");
          qDebug()<<"SLEEP";
          //ret=system("init 1");
          //syslog( LOG_ERR, "Power OFF, init ret %d",ret);
          butt_state=0;
        }
      }
    }
//      msleep(20);

      //getGPIO(ACT,&val);
      //qDebug()<<"GPIO ACT"<<val;
      //getGPIO(SLP,&val);
      //qDebug()<<"GPIO SLP"<<val;
      //getGPIO(GPIO7,&val1); getGPIO(GPIO6,&val2);getGPIO(GPIO5,&val3);getGPIO(GPIO4,&val4);
      //qDebug()<<"GPIO C2 C3 C4 C5 "<<val1<<val2<<val3<<val4;

    }
    loop->processEvents();
    //msleep(2000);
    if(abort) { return;}
  }
}
//------------------------------------------------- Common functions ---------------------------------


/****************************************************************
 * initialisation of bbb hardware (SPI, GPIO)
 ****************************************************************/
int TGpio::initalHW(void)
{
  qDebug()<<"Initialisation Base Board";
  return 0;
}

//------------------------------------------------- Host GPIO functions ---------------------------------
/****************************************************************
 * gpio_export
 ****************************************************************/
int TGpio::pins_export(void)
{
  qDebug()<<"Set pins export";
  if(gpio_export(GPIO0)) return 1;
  if(gpio_export(GPIO1)) return 1;
  if(gpio_export(GPIO2)) return 1;
  if(gpio_export(GPIO3)) return 1;
  if(gpio_export(GPIO4)) return 1;
  if(gpio_export(GPIO5)) return 1;
  if(gpio_export(GPIO6)) return 1;
  if(gpio_export(GPIO7)) return 1;
 // if(gpio_export(SLP))   return 1;
  if(gpio_export(ACT))   return 1;

  return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int TGpio::pins_unexport(void)
{
    qDebug()<<"Set pins unexport";
    gpio_unexport(GPIO0);
    gpio_unexport(GPIO1);
    gpio_unexport(GPIO2);
    gpio_unexport(GPIO3);
    gpio_unexport(GPIO4);
    gpio_unexport(GPIO5);
    gpio_unexport(GPIO6);
    gpio_unexport(GPIO7);
 //   gpio_unexport(SLP);
    gpio_unexport(ACT);

    return 0;
}

// 0 - input ,1 - output pin
int TGpio::setDirGPIO(PIN_GPIOS GPIO,unsigned int dir)
{
  return gpio_set_dir(GPIO,(PIN_DIRECTION) dir);
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int TGpio::setGPIO(PIN_GPIOS GPIO,unsigned int val)
{
  return gpio_set_value(GPIO,(PIN_VALUE) val);
}

int TGpio::getGPIO(PIN_GPIOS GPIO,unsigned  int *val)
{

   return gpio_get_value(GPIO,val);
}

int TGpio::init_gpios(void)
{
    int n;

    setDirGPIO(GPIO1,OUTPUT_PIN);
    setDirGPIO(GPIO2,OUTPUT_PIN);
    setGPIO(GPIO1,0); // set output enable
    setGPIO(GPIO2,0); // normal USB mode
    epfd = epoll_create(5);

    gpio_set_dir(GPIO7,INPUT_PIN);
    gpio_set_dir(GPIO6,INPUT_PIN);
    gpio_set_dir(GPIO5,INPUT_PIN);
    gpio_set_dir(GPIO4,INPUT_PIN);

    gpio_set_dir(ACT,INPUT_PIN);
  //  gpio_set_dir(SLP,INPUT_PIN);
    gpio_set_edge(ACT,(char*)"both");
  //  gpio_set_edge(SLP,(char*)"both");
    fd_d0=gpio_fd_open(ACT);
  //  fd_d1=gpio_fd_open(SLP);
    ev_d0.events = EPOLLET|EPOLLIN;
    ev_d1.events = EPOLLET|EPOLLIN;

    ev_d0.data.fd = fd_d0;
    ev_d1.data.fd = fd_d1;

    n = epoll_ctl(epfd, EPOLL_CTL_ADD, fd_d0, &ev_d0);
    if (n != 0) {
      fprintf(stderr, "epoll_ctl returned %d: %s\n", n, strerror(errno));
      return 1;
    }

    n = epoll_ctl(epfd, EPOLL_CTL_ADD, fd_d1, &ev_d1);
    if (n != 0) {
      printf("epoll_ctl returned %d: %s\n", n, strerror(errno));
      return 1;
    }
    return 0;
}

void TGpio::close_gpios(void)
{
  if(fd_d0) close(fd_d0);
  if(fd_d1) close(fd_d1);

}
unsigned int TGpio::polling_gpios(int timePolling)
{
    unsigned int value = 0 ,n,i;

    n = epoll_wait(epfd, events, 5, timePolling);

    for ( i = 0;  i < n; ++i) {
      if (events[i].data.fd == ev_d0.data.fd) { value|=1;}
      if (events[i].data.fd == ev_d1.data.fd) { value|=2;}
    }
    return value;
}

void  TGpio::slot_setBackLight(char *val)
{
  gpio_set_backlight(val);
}

void  TGpio::slot_setInterfaces(bool val)
{
  gpio_set_interfaces(val);
}

// SimpleGPIO.cpp functions
/****************************************************************
 * gpio_export
 ****************************************************************/
int gpio_export(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);

    return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int gpio_unexport(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    write(fd, buf, len);
    close(fd);
    return 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int gpio_set_dir(unsigned int gpio, PIN_DIRECTION out_flag)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/direction");
        return fd;
    }

    if (out_flag == OUTPUT_PIN)
        write(fd, "out", 4);
    else
        write(fd, "in", 3);

    close(fd);
    return 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int gpio_set_value(unsigned int gpio, PIN_VALUE value)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/set-value");
        return fd;
    }

    if (value==LOW)
        write(fd, "0", 2);
    else
        write(fd, "1", 2);

    close(fd);
    return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int gpio_get_value(unsigned int gpio, unsigned int *value)
{
    int fd;
    char buf[MAX_BUF];
    char ch;

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_RDONLY);
    if (fd < 0) {
        perror("gpio/get-value");
        return fd;
    }

    read(fd, &ch, 1);

    if (ch != '0') {
        *value = 1;
    } else {
        *value = 0;
    }

    close(fd);
    return 0;
}


/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int gpio_set_edge(unsigned int gpio, char *edge)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("gpio/set-edge");
        return fd;
    }

    write(fd, edge, strlen(edge) + 1);
    close(fd);
    return 0;
}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int gpio_fd_open(unsigned int gpio)
{
    int fd;
    char buf[MAX_BUF];

    snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_RDONLY | O_NONBLOCK );
    if (fd < 0) {
        perror("gpio/fd_open");
    }
    return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int gpio_fd_close(int fd)
{
    return close(fd);
}

/****************************************************************
 * gpio_set_backlight
 ****************************************************************/
void gpio_set_backlight(char *val)
{
  int fd;
  char buf[255];
  snprintf(buf, sizeof(buf), "/sys/class/backlight/backlight/brightness");
  fd = open(buf, O_WRONLY);
  if (fd < 0) {
    perror("gpio/backlight");
    return;
  }
  write(fd, val, sizeof(val));
  close(fd);
}
/****************************************************************
 * gpio_set_interfaces
 ****************************************************************/
void gpio_set_interfaces(bool val)
{

  if(val)
    gpio_set_value(GPIO2,HIGH); // simple USB mode
  else
    gpio_set_value(GPIO2,LOW); // normal USB mode
}
