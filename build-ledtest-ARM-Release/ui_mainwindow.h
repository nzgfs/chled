/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *LED2;
    QPushButton *LED4;
    QPushButton *move;
    QPushButton *rotate;
    QPushButton *stop;
    QPushButton *exit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;
    QToolBar *toolBar_2;
    QToolBar *toolBar_3;
    QToolBar *toolBar_4;
    QToolBar *toolBar_5;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(656, 437);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        LED2 = new QPushButton(centralWidget);
        LED2->setObjectName(QStringLiteral("LED2"));
        LED2->setGeometry(QRect(80, 50, 111, 101));
        QFont font;
        font.setPointSize(15);
        LED2->setFont(font);
        LED2->setAutoDefault(false);
        LED2->setDefault(false);
        LED2->setFlat(false);
        LED4 = new QPushButton(centralWidget);
        LED4->setObjectName(QStringLiteral("LED4"));
        LED4->setGeometry(QRect(80, 200, 111, 101));
        LED4->setFont(font);
        move = new QPushButton(centralWidget);
        move->setObjectName(QStringLiteral("move"));
        move->setGeometry(QRect(290, 80, 121, 71));
        move->setFont(font);
        rotate = new QPushButton(centralWidget);
        rotate->setObjectName(QStringLiteral("rotate"));
        rotate->setGeometry(QRect(460, 80, 121, 71));
        rotate->setFont(font);
        stop = new QPushButton(centralWidget);
        stop->setObjectName(QStringLiteral("stop"));
        stop->setGeometry(QRect(290, 200, 121, 71));
        stop->setFont(font);
        exit = new QPushButton(centralWidget);
        exit->setObjectName(QStringLiteral("exit"));
        exit->setGeometry(QRect(460, 200, 121, 71));
        exit->setFont(font);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 656, 28));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar_2 = new QToolBar(MainWindow);
        toolBar_2->setObjectName(QStringLiteral("toolBar_2"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_2);
        toolBar_3 = new QToolBar(MainWindow);
        toolBar_3->setObjectName(QStringLiteral("toolBar_3"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_3);
        toolBar_4 = new QToolBar(MainWindow);
        toolBar_4->setObjectName(QStringLiteral("toolBar_4"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_4);
        toolBar_5 = new QToolBar(MainWindow);
        toolBar_5->setObjectName(QStringLiteral("toolBar_5"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_5);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        LED2->setText(QApplication::translate("MainWindow", "LED2", 0));
        LED4->setText(QApplication::translate("MainWindow", "LED4", 0));
        move->setText(QApplication::translate("MainWindow", "Move", 0));
        rotate->setText(QApplication::translate("MainWindow", "Rotate", 0));
        stop->setText(QApplication::translate("MainWindow", "Stop", 0));
        exit->setText(QApplication::translate("MainWindow", "Exit", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
        toolBar_2->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", 0));
        toolBar_3->setWindowTitle(QApplication::translate("MainWindow", "toolBar_3", 0));
        toolBar_4->setWindowTitle(QApplication::translate("MainWindow", "toolBar_4", 0));
        toolBar_5->setWindowTitle(QApplication::translate("MainWindow", "toolBar_5", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
