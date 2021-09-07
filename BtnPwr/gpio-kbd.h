#ifndef GPIOKBD_H
#define GPIOKBD_H

#include <QtCore>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <linux/input.h>

/*!
 * \file gpio-kbd.h
 * \brief Заголовок описания класса TkbdGpio.
 *
 * Файл содержит определения класса TkbdGpio.
 */

/****************************************************************
* Constants
****************************************************************/
///@brief Путь к системному событию нажатия на кнопки.
#define INPUT_EVENT "/dev/input/event1"
///@brief Время через которое нажатие определяется как длинное.
#define PRESS_TIMEOUT1 2 /* 2 seconds  sleep */
#define PRESS_TIMEOUT2 5 /* 5 seconds  shutdown */
/*!
 * \brief Используется для определения нажатия пяти кнопокплаты IMX6_1.
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
class TkbdGpio: public QThread
{
  Q_OBJECT
  public:

    TkbdGpio();
    ~TkbdGpio();
/*!
 * Возвращает код ошибки.
 * \return Код возврата: 0 - нет ошибок, 1 - ошибка создания
 * обработчика событий, 2 - ошибка открытия дескриптора входных событий,
 * 3 - ошибка конфигурирования обработчика событий.
 */
    int getError(void) { return error; }
signals:
    void keyPressed(int);
private:
    int initalKBD(void);
    void releaseKBD(void);
protected:
    void run();
private:
    int error;
    int kev_ev;
    int fd_ev;
    struct epoll_event ev_ev;
    struct input_event keydat;
    bool abort;
};

#endif /* GPIOKBD_H */
