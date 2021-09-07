
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

#include <QtCore>

/****************************************************************
 * Object constructor
 ****************************************************************/
TGpio::TGpio()
{
  qDebug()<<  "TGpio object constructor start.";  
  qDebug()<<  "TGpio object constructor finish."; 
}

/****************************************************************
 * destructor
 * Unexport pins and free spi1
 ****************************************************************/
TGpio::~TGpio()
{
  qDebug()<<  "TGpio object destructor start.";
  pins_unexport();
  qDebug()<< "TGpio object destructor finish.";
}


//------------------------------------------------- Common functions ---------------------------------


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
    gpio_unexport(GPIO1);

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

    setDirGPIO(GPIO1,OUTPUT_PIN);
    setDirGPIO(GPIO2,OUTPUT_PIN);
    setGPIO(GPIO1,0); // set output enable
    setGPIO(GPIO2,0); // normal USB mode

    return 0;
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
