#include "mainwindow.h"
#include "ui_mainwindow.h"
//Chen Hang
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fd_LED = open(LED, O_RDWR);
    flag=0;
    ui->LED2->setStyleSheet("background-color: rgb(180, 180, 180); color: rgb(255, 255, 255)");
    ui->LED4->setStyleSheet("background-color: rgb(180, 180, 180); color: rgb(255, 255, 255)");
    state[0]=0;
    state[1]=0;
    rtargs[0]=0;
    rtargs[1]=500;
}

MainWindow::~MainWindow()
{
    delete ui;


}

void MainWindow::on_LED2_clicked()
{
    FS4412_LED_switch(fd_LED,1);
    if(state[0]==0) state[0]=1;
    else state[0]=0;
    color();
}

void MainWindow::on_LED4_clicked()
{
    FS4412_LED_switch(fd_LED,2);
    if(state[1]==0) state[1]=1;
    else state[1]=0;
    color();
}

void MainWindow::color()
{

    if(state[0]==1) ui->LED2->setStyleSheet("background-color: rgb(0, 0, 250); color: rgb(255, 255, 255)");
    else if(state[0]==0) ui->LED2->setStyleSheet("background-color: rgb(180, 180, 180); color: rgb(255, 255, 255)");

    if(state[1]==1) ui->LED4->setStyleSheet("background-color: rgb(0, 0, 250); color: rgb(255, 255, 255)");
    else if(state[1]==0) ui->LED4->setStyleSheet("background-color: rgb(180, 180, 180); color: rgb(255, 255, 255)");

}

void MainWindow::on_move_clicked()
{
    if(state[0]!=state[1])
    {
        buf[0]=state[0];
        buf[1]=state[1];
        rtargs[0]=1;
        FS4412_LED_rotate(fd_LED,rtargs);
        state[0]=buf[1];
        state[1]=buf[0];
        color();
    }

}

void MainWindow::on_rotate_clicked()
{
    flag=1;
    while(flag)
    {
        on_move_clicked();
        delay();
    }
}

void MainWindow::on_stop_clicked()
{
    flag=0;
}

void MainWindow::delay()
{
    QEventLoop evloop;
    QTimer::singleShot(1000, &evloop, SLOT(quit()));
    evloop.exec();
}

void MainWindow::on_exit_clicked()
{
    QWidget::close();
    QApplication* app;
    app->exit(0);
}
