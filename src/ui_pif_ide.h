/********************************************************************************
** Form generated from reading UI file 'pif_ide.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PIF_IDE_H
#define UI_PIF_IDE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Pif_IDEClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *Pif_IDEClass)
    {
        if (Pif_IDEClass->objectName().isEmpty())
            Pif_IDEClass->setObjectName(QStringLiteral("Pif_IDEClass"));
        Pif_IDEClass->resize(600, 400);
        centralWidget = new QWidget(Pif_IDEClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Pif_IDEClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Pif_IDEClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        Pif_IDEClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(Pif_IDEClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Pif_IDEClass->setStatusBar(statusBar);
        mainToolBar = new QToolBar(Pif_IDEClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setContextMenuPolicy(Qt::NoContextMenu);
        Pif_IDEClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        retranslateUi(Pif_IDEClass);

        QMetaObject::connectSlotsByName(Pif_IDEClass);
    } // setupUi

    void retranslateUi(QMainWindow *Pif_IDEClass)
    {
        Pif_IDEClass->setWindowTitle(QApplication::translate("Pif_IDEClass", "Pif_IDE", 0));
        mainToolBar->setWindowTitle(QApplication::translate("Pif_IDEClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class Pif_IDEClass: public Ui_Pif_IDEClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PIF_IDE_H
