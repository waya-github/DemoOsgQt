/********************************************************************************
** Form generated from reading UI file 'DemoOsgQt.ui'
**
** Created by: Qt User Interface Compiler version 5.9.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEMOOSGQT_H
#define UI_DEMOOSGQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "osgviewerwidget.h"

QT_BEGIN_NAMESPACE

class Ui_DemoOsgQtClass
{
public:
    QWidget *centralWidget;
    OsgViewerWidget *openGLWidgetOSGB;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *pbt_001_ReadOsgbAndObjToViewer;
    QPushButton *pbt_002_DrawPolygon;
    QPushButton *pbt_000_InitializeOsgViewer;
    QWidget *tab_2;
    QTextBrowser *textBrowser;
    OsgViewerWidget *openGLWidgetOBJ;
    OsgViewerWidget *openGLWidgetShowMe;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DemoOsgQtClass)
    {
        if (DemoOsgQtClass->objectName().isEmpty())
            DemoOsgQtClass->setObjectName(QStringLiteral("DemoOsgQtClass"));
        DemoOsgQtClass->resize(1414, 768);
        centralWidget = new QWidget(DemoOsgQtClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        openGLWidgetOSGB = new OsgViewerWidget(centralWidget);
        openGLWidgetOSGB->setObjectName(QStringLiteral("openGLWidgetOSGB"));
        openGLWidgetOSGB->setGeometry(QRect(190, 19, 400, 391));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 181, 711));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        pbt_001_ReadOsgbAndObjToViewer = new QPushButton(tab);
        pbt_001_ReadOsgbAndObjToViewer->setObjectName(QStringLiteral("pbt_001_ReadOsgbAndObjToViewer"));
        pbt_001_ReadOsgbAndObjToViewer->setGeometry(QRect(0, 20, 171, 23));
        pbt_001_ReadOsgbAndObjToViewer->setLayoutDirection(Qt::LeftToRight);
        pbt_002_DrawPolygon = new QPushButton(tab);
        pbt_002_DrawPolygon->setObjectName(QStringLiteral("pbt_002_DrawPolygon"));
        pbt_002_DrawPolygon->setGeometry(QRect(0, 40, 171, 23));
        pbt_000_InitializeOsgViewer = new QPushButton(tab);
        pbt_000_InitializeOsgViewer->setObjectName(QStringLiteral("pbt_000_InitializeOsgViewer"));
        pbt_000_InitializeOsgViewer->setGeometry(QRect(0, 0, 171, 23));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(190, 430, 1221, 281));
        openGLWidgetOBJ = new OsgViewerWidget(centralWidget);
        openGLWidgetOBJ->setObjectName(QStringLiteral("openGLWidgetOBJ"));
        openGLWidgetOBJ->setGeometry(QRect(600, 19, 400, 391));
        openGLWidgetShowMe = new OsgViewerWidget(centralWidget);
        openGLWidgetShowMe->setObjectName(QStringLiteral("openGLWidgetShowMe"));
        openGLWidgetShowMe->setGeometry(QRect(1010, 19, 400, 391));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(370, 7, 54, 12));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(783, 7, 54, 12));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(1194, 7, 54, 12));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(190, 414, 54, 12));
        DemoOsgQtClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DemoOsgQtClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1414, 23));
        DemoOsgQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DemoOsgQtClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        DemoOsgQtClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DemoOsgQtClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DemoOsgQtClass->setStatusBar(statusBar);

        retranslateUi(DemoOsgQtClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DemoOsgQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *DemoOsgQtClass)
    {
        DemoOsgQtClass->setWindowTitle(QApplication::translate("DemoOsgQtClass", "DemoOsgQt", Q_NULLPTR));
        pbt_001_ReadOsgbAndObjToViewer->setText(QApplication::translate("DemoOsgQtClass", "001_\350\257\273\345\217\226Osgb\345\222\214Obj\346\226\207\344\273\266     ", Q_NULLPTR));
        pbt_002_DrawPolygon->setText(QApplication::translate("DemoOsgQtClass", "002_\347\273\230\345\210\266\344\270\211\347\273\264\345\244\232\350\276\271\345\275\242        ", Q_NULLPTR));
        pbt_000_InitializeOsgViewer->setText(QApplication::translate("DemoOsgQtClass", "000_\346\270\205\347\251\272\350\277\230\345\216\237\345\234\272\346\231\257          ", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("DemoOsgQtClass", "\347\251\272\351\227\264\351\207\217\346\265\213", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("DemoOsgQtClass", "\344\270\211\347\273\264\346\240\207\346\263\250", Q_NULLPTR));
        label->setText(QApplication::translate("DemoOsgQtClass", "OSGB", Q_NULLPTR));
        label_2->setText(QApplication::translate("DemoOsgQtClass", "OBJ", Q_NULLPTR));
        label_3->setText(QApplication::translate("DemoOsgQtClass", "\345\205\266\344\273\226", Q_NULLPTR));
        label_4->setText(QApplication::translate("DemoOsgQtClass", "\346\265\213\350\257\225\350\276\223\345\207\272", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DemoOsgQtClass: public Ui_DemoOsgQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEMOOSGQT_H
