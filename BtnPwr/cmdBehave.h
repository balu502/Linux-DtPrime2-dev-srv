#ifndef CMD_H
#define CMD_H

#include <QtCore>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <linux/input.h>

/*!
 * \file cmdBehave.h
 * \brief Заголовок описания класса TcmdBehav.
 *
 * Файл содержит определения класса TcmdBehav.
 */

/*!
 * \brief Используется для определения нажатия пяти кнопокплаты IMX6_1.

 */
class TcmdBehav: public QThread
{
  Q_OBJECT
  public:

    TcmdBehav();
    ~TcmdBehav();
protected:
    void run();
private:
    bool abort;
};

#endif /* GPIOKBD_H */
