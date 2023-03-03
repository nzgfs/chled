#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTimer"
#include "QWidget"
#include "QApplication"
#include "QEventLoop"
//ifndef _EX3_API_H
#define _EX3_API_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>

/* path of devices */
#define LED ("/dev/FS4412_LED")

#define LED_MAGIC 'L'

#define LED_SWITCH  _IOW(LED_MAGIC, 1, uint8_t)
#define LED_ROTATE	_IOW(LED_MAGIC, 2, size_t)


/**********************************************************************
 * @Function:	FS4412_LED_switch
 * @Description:	switch on/off LED
 * @Param[in]:	(int)fd:
 * 					file descriptor of LED device,
 * 				(uint_8)led:
 * 					LED number
 * @Return:	other than -1 upon successful completion
 **********************************************************************/
#define FS4412_LED_switch(fd, led) 		ioctl(fd, LED_SWITCH, led)

/**********************************************************************
 * @Function:	FS4412_LED_rotate
 * @Description:	move LED
 * @Param[in]:	(int)fd:
 * 					file descriptor of LED device,
 * 				(size_t)rt_args[2]:
 *					rt_args[0]:	0: stop rotating
 *								1: move anticlockwise
 *								2: move clockwise
 *								3: rotate anticlockwise
 *								4: rotate clockwise
 *					rt_args[1]: rotation delay (ms)
 * @Return:	other than -1 upon successful completion
 **********************************************************************/
#define FS4412_LED_rotate(fd, rt_args) 	ioctl(fd, LED_ROTATE, rt_args)


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_LED2_clicked();

    void on_LED4_clicked();

    void color();

    void on_move_clicked();

    void on_rotate_clicked();

    void on_stop_clicked();

    void delay();

    void on_exit_clicked();

private:
    Ui::MainWindow *ui;
    int flag;
    int fd_LED;
    uint8_t state[2];
    uint8_t temp[2];
    uint8_t buf[2];
    size_t rtargs[2];
};

#endif // MAINWINDOW_H
