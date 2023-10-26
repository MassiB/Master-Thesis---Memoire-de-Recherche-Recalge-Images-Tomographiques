/********************************************************************************
** Form generated from reading UI file 'tomoregistration.ui'
**
** Created: Wed Nov 13 17:47:52 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOMOREGISTRATION_H
#define UI_TOMOREGISTRATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_TomoRegistrationClass
{
public:
    QAction *actionSave_As;
    QAction *action;
    QAction *action_2;
    QAction *actionExit;
    QAction *actionSource;
    QAction *actionTarget;
    QAction *actionProfile_Profile;
    QAction *actionProfile_PET;
    QAction *actionFiducialMarkers;
    QAction *actionData_Processing;
    QAction *actionSettings;
    QAction *action_VTK;
    QAction *action_ITK;
    QAction *action3D;
    QAction *actionSurface;
    QAction *actionApply;
    QAction *actionReset;
    QAction *actionMm;
    QAction *actionInch;
    QAction *actionScreenShot;
    QAction *actionSeriesDicom;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QVTKWidget *qvtkWidget1;
    QVTKWidget *qvtkWidget2;
    QVTKWidget *qvtkWidget3;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuOpen;
    QMenu *menuTools;
    QToolBar *DicomSeriesToolBar;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TomoRegistrationClass)
    {
        if (TomoRegistrationClass->objectName().isEmpty())
            TomoRegistrationClass->setObjectName(QString::fromUtf8("TomoRegistrationClass"));
        TomoRegistrationClass->resize(917, 553);
        TomoRegistrationClass->setMinimumSize(QSize(917, 553));
        TomoRegistrationClass->setAcceptDrops(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icon/icon/Mouse.png"), QSize(), QIcon::Normal, QIcon::Off);
        TomoRegistrationClass->setWindowIcon(icon);
        TomoRegistrationClass->setStyleSheet(QString::fromUtf8("#statusBar\n"
"{\n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:1, y2:0.114, stop:0 rgba(171, 171, 171, 173), stop:1 rgba(255, 255, 255, 173));\n"
"}"));
        actionSave_As = new QAction(TomoRegistrationClass);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icon/icon/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_As->setIcon(icon1);
        action = new QAction(TomoRegistrationClass);
        action->setObjectName(QString::fromUtf8("action"));
        action_2 = new QAction(TomoRegistrationClass);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        actionExit = new QAction(TomoRegistrationClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Icon/icon/logout.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon2);
        actionSource = new QAction(TomoRegistrationClass);
        actionSource->setObjectName(QString::fromUtf8("actionSource"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Icon/icon/S.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSource->setIcon(icon3);
        actionTarget = new QAction(TomoRegistrationClass);
        actionTarget->setObjectName(QString::fromUtf8("actionTarget"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Icon/icon/T.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTarget->setIcon(icon4);
        actionProfile_Profile = new QAction(TomoRegistrationClass);
        actionProfile_Profile->setObjectName(QString::fromUtf8("actionProfile_Profile"));
        actionProfile_Profile->setCheckable(true);
        actionProfile_Profile->setChecked(true);
        actionProfile_PET = new QAction(TomoRegistrationClass);
        actionProfile_PET->setObjectName(QString::fromUtf8("actionProfile_PET"));
        actionProfile_PET->setCheckable(true);
        actionFiducialMarkers = new QAction(TomoRegistrationClass);
        actionFiducialMarkers->setObjectName(QString::fromUtf8("actionFiducialMarkers"));
        actionFiducialMarkers->setCheckable(true);
        actionData_Processing = new QAction(TomoRegistrationClass);
        actionData_Processing->setObjectName(QString::fromUtf8("actionData_Processing"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/Icon/icon/processing_logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionData_Processing->setIcon(icon5);
        actionSettings = new QAction(TomoRegistrationClass);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/Icon/icon/Settings-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSettings->setIcon(icon6);
        action_VTK = new QAction(TomoRegistrationClass);
        action_VTK->setObjectName(QString::fromUtf8("action_VTK"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/Icon/icon/VTK.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_VTK->setIcon(icon7);
        action_ITK = new QAction(TomoRegistrationClass);
        action_ITK->setObjectName(QString::fromUtf8("action_ITK"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/Icon/icon/ITK-PNG.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_ITK->setIcon(icon8);
        action3D = new QAction(TomoRegistrationClass);
        action3D->setObjectName(QString::fromUtf8("action3D"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/Icon/icon/3D Icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        action3D->setIcon(icon9);
        actionSurface = new QAction(TomoRegistrationClass);
        actionSurface->setObjectName(QString::fromUtf8("actionSurface"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/Icon/icon/250px-Marching_tetrahedrons.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSurface->setIcon(icon10);
        actionApply = new QAction(TomoRegistrationClass);
        actionApply->setObjectName(QString::fromUtf8("actionApply"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/Icon/icon/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionApply->setIcon(icon11);
        actionReset = new QAction(TomoRegistrationClass);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/Icon/icon/Reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReset->setIcon(icon12);
        actionMm = new QAction(TomoRegistrationClass);
        actionMm->setObjectName(QString::fromUtf8("actionMm"));
        actionMm->setCheckable(true);
        actionMm->setChecked(false);
        actionInch = new QAction(TomoRegistrationClass);
        actionInch->setObjectName(QString::fromUtf8("actionInch"));
        actionInch->setCheckable(true);
        actionScreenShot = new QAction(TomoRegistrationClass);
        actionScreenShot->setObjectName(QString::fromUtf8("actionScreenShot"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/Icon/icon/Screenshot.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionScreenShot->setIcon(icon13);
        actionSeriesDicom = new QAction(TomoRegistrationClass);
        actionSeriesDicom->setObjectName(QString::fromUtf8("actionSeriesDicom"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/Icon/icon/ImageDocument.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSeriesDicom->setIcon(icon14);
        centralWidget = new QWidget(TomoRegistrationClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter_2->sizePolicy().hasHeightForWidth());
        splitter_2->setSizePolicy(sizePolicy);
        splitter_2->setLineWidth(1);
        splitter_2->setMidLineWidth(0);
        splitter_2->setOrientation(Qt::Vertical);
        splitter_2->setOpaqueResize(false);
        splitter_2->setHandleWidth(2);
        splitter_2->setChildrenCollapsible(false);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy1);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setOpaqueResize(false);
        splitter->setHandleWidth(2);
        splitter->setChildrenCollapsible(false);
        qvtkWidget1 = new QVTKWidget(splitter);
        qvtkWidget1->setObjectName(QString::fromUtf8("qvtkWidget1"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(qvtkWidget1->sizePolicy().hasHeightForWidth());
        qvtkWidget1->setSizePolicy(sizePolicy2);
        splitter->addWidget(qvtkWidget1);
        qvtkWidget2 = new QVTKWidget(splitter);
        qvtkWidget2->setObjectName(QString::fromUtf8("qvtkWidget2"));
        sizePolicy2.setHeightForWidth(qvtkWidget2->sizePolicy().hasHeightForWidth());
        qvtkWidget2->setSizePolicy(sizePolicy2);
        splitter->addWidget(qvtkWidget2);
        splitter_2->addWidget(splitter);
        qvtkWidget3 = new QVTKWidget(splitter_2);
        qvtkWidget3->setObjectName(QString::fromUtf8("qvtkWidget3"));
        sizePolicy2.setHeightForWidth(qvtkWidget3->sizePolicy().hasHeightForWidth());
        qvtkWidget3->setSizePolicy(sizePolicy2);
        qvtkWidget3->setMinimumSize(QSize(0, 0));
        splitter_2->addWidget(qvtkWidget3);

        gridLayout->addWidget(splitter_2, 0, 0, 1, 1);

        TomoRegistrationClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TomoRegistrationClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 917, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFile->setCursor(QCursor(Qt::PointingHandCursor));
        menuOpen = new QMenu(menuFile);
        menuOpen->setObjectName(QString::fromUtf8("menuOpen"));
        menuOpen->setCursor(QCursor(Qt::PointingHandCursor));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/Icon/icon/folder2.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuOpen->setIcon(icon15);
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuTools->setCursor(QCursor(Qt::PointingHandCursor));
        TomoRegistrationClass->setMenuBar(menuBar);
        DicomSeriesToolBar = new QToolBar(TomoRegistrationClass);
        DicomSeriesToolBar->setObjectName(QString::fromUtf8("DicomSeriesToolBar"));
        DicomSeriesToolBar->setMovable(false);
        DicomSeriesToolBar->setIconSize(QSize(30, 20));
        TomoRegistrationClass->addToolBar(Qt::TopToolBarArea, DicomSeriesToolBar);
        toolBar = new QToolBar(TomoRegistrationClass);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setLayoutDirection(Qt::LeftToRight);
        toolBar->setMovable(false);
        toolBar->setIconSize(QSize(50, 20));
        toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolBar->setFloatable(true);
        TomoRegistrationClass->addToolBar(Qt::TopToolBarArea, toolBar);
        statusBar = new QStatusBar(TomoRegistrationClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setSizeGripEnabled(true);
        TomoRegistrationClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuFile->addAction(menuOpen->menuAction());
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuOpen->addAction(actionSource);
        menuOpen->addAction(actionTarget);
        menuTools->addAction(actionData_Processing);
        menuTools->addSeparator();
        menuTools->addAction(actionSettings);
        DicomSeriesToolBar->addAction(actionSeriesDicom);
        DicomSeriesToolBar->addAction(action3D);
        DicomSeriesToolBar->addAction(actionSurface);

        retranslateUi(TomoRegistrationClass);
        QObject::connect(actionExit, SIGNAL(triggered()), TomoRegistrationClass, SLOT(close()));
        QObject::connect(action3D, SIGNAL(triggered()), TomoRegistrationClass, SLOT(Get3D()));
        QObject::connect(actionApply, SIGNAL(triggered()), TomoRegistrationClass, SLOT(ApplyRegistration()));
        QObject::connect(actionReset, SIGNAL(triggered()), TomoRegistrationClass, SLOT(Reset()));
        QObject::connect(actionSave_As, SIGNAL(triggered()), TomoRegistrationClass, SLOT(Save()));
        QObject::connect(actionData_Processing, SIGNAL(triggered()), TomoRegistrationClass, SLOT(OpenDataProcess()));
        QObject::connect(actionSettings, SIGNAL(triggered()), TomoRegistrationClass, SLOT(OpenSettings()));
        QObject::connect(actionSource, SIGNAL(triggered()), TomoRegistrationClass, SLOT(OpenSource()));
        QObject::connect(actionTarget, SIGNAL(triggered()), TomoRegistrationClass, SLOT(OpenTarget()));
        QObject::connect(actionSurface, SIGNAL(triggered()), TomoRegistrationClass, SLOT(GetSurface()));
        QObject::connect(actionExit, SIGNAL(triggered()), TomoRegistrationClass, SLOT(exit()));
        QObject::connect(actionSeriesDicom, SIGNAL(triggered()), TomoRegistrationClass, SLOT(DisplayDicomSeries()));

        QMetaObject::connectSlotsByName(TomoRegistrationClass);
    } // setupUi

    void retranslateUi(QMainWindow *TomoRegistrationClass)
    {
        TomoRegistrationClass->setWindowTitle(QApplication::translate("TomoRegistrationClass", "TomoRegistration by Massinissa Bandou", 0, QApplication::UnicodeUTF8));
        actionSave_As->setText(QApplication::translate("TomoRegistrationClass", "Save As...", 0, QApplication::UnicodeUTF8));
        action->setText(QString());
        action_2->setText(QString());
        actionExit->setText(QApplication::translate("TomoRegistrationClass", "&Exit", 0, QApplication::UnicodeUTF8));
        actionSource->setText(QApplication::translate("TomoRegistrationClass", "&Source", 0, QApplication::UnicodeUTF8));
        actionTarget->setText(QApplication::translate("TomoRegistrationClass", "&Target", 0, QApplication::UnicodeUTF8));
        actionProfile_Profile->setText(QApplication::translate("TomoRegistrationClass", "Profile / Profile", 0, QApplication::UnicodeUTF8));
        actionProfile_PET->setText(QApplication::translate("TomoRegistrationClass", "Profile / PET", 0, QApplication::UnicodeUTF8));
        actionFiducialMarkers->setText(QApplication::translate("TomoRegistrationClass", "Fiducial Markers", 0, QApplication::UnicodeUTF8));
        actionData_Processing->setText(QApplication::translate("TomoRegistrationClass", "Data Process...", 0, QApplication::UnicodeUTF8));
        actionSettings->setText(QApplication::translate("TomoRegistrationClass", "Settings...", 0, QApplication::UnicodeUTF8));
        action_VTK->setText(QApplication::translate("TomoRegistrationClass", "    VTK", 0, QApplication::UnicodeUTF8));
        action_ITK->setText(QApplication::translate("TomoRegistrationClass", "     ITK", 0, QApplication::UnicodeUTF8));
        action3D->setText(QApplication::translate("TomoRegistrationClass", "3D", 0, QApplication::UnicodeUTF8));
        actionSurface->setText(QApplication::translate("TomoRegistrationClass", "Surface", 0, QApplication::UnicodeUTF8));
        actionApply->setText(QApplication::translate("TomoRegistrationClass", "Apply", 0, QApplication::UnicodeUTF8));
        actionReset->setText(QApplication::translate("TomoRegistrationClass", "Reset", 0, QApplication::UnicodeUTF8));
        actionMm->setText(QApplication::translate("TomoRegistrationClass", "inch to mm", 0, QApplication::UnicodeUTF8));
        actionInch->setText(QApplication::translate("TomoRegistrationClass", "inch", 0, QApplication::UnicodeUTF8));
        actionScreenShot->setText(QApplication::translate("TomoRegistrationClass", "ScreenShot", 0, QApplication::UnicodeUTF8));
        actionSeriesDicom->setText(QApplication::translate("TomoRegistrationClass", "SeriesDicom", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("TomoRegistrationClass", "&File", 0, QApplication::UnicodeUTF8));
        menuOpen->setTitle(QApplication::translate("TomoRegistrationClass", "Open", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("TomoRegistrationClass", "&Tools", 0, QApplication::UnicodeUTF8));
        DicomSeriesToolBar->setWindowTitle(QApplication::translate("TomoRegistrationClass", "registration toolbar", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("TomoRegistrationClass", "dicom toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TomoRegistrationClass: public Ui_TomoRegistrationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOMOREGISTRATION_H
