/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
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
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_Image;
    QWidget *centralWidget;
    QSlider *scfSlider;
    QToolButton *goTo100;
    QToolButton *tbFull;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(609, 300);
        actionOpen_Image = new QAction(MainWindow);
        actionOpen_Image->setObjectName(QStringLiteral("actionOpen_Image"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        scfSlider = new QSlider(centralWidget);
        scfSlider->setObjectName(QStringLiteral("scfSlider"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scfSlider->sizePolicy().hasHeightForWidth());
        scfSlider->setSizePolicy(sizePolicy);
        scfSlider->setMinimum(1);
        scfSlider->setMaximum(256);
        scfSlider->setPageStep(1);
        scfSlider->setValue(16);
        scfSlider->setTracking(true);
        scfSlider->setOrientation(Qt::Horizontal);
        scfSlider->setInvertedAppearance(false);
        scfSlider->setInvertedControls(false);
        scfSlider->setTickPosition(QSlider::NoTicks);
        scfSlider->setTickInterval(2);
        goTo100 = new QToolButton(centralWidget);
        goTo100->setObjectName(QStringLiteral("goTo100"));
        goTo100->setGeometry(QRect(280, 70, 41, 21));
        tbFull = new QToolButton(centralWidget);
        tbFull->setObjectName(QStringLiteral("tbFull"));
        tbFull->setGeometry(QRect(320, 70, 28, 21));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 609, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_Image);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Simple image viewer", 0));
        actionOpen_Image->setText(QApplication::translate("MainWindow", "Open Image", 0));
        goTo100->setText(QApplication::translate("MainWindow", "100%", 0));
        tbFull->setText(QApplication::translate("MainWindow", "Full", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
