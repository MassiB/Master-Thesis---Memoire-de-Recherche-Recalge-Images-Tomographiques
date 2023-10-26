/********************************************************************************
** Form generated from reading UI file 'DataDimension.ui'
**
** Created: Sat Nov 16 15:02:10 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATADIMENSION_H
#define UI_DATADIMENSION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_DataDimension
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *SurfaceGroupBox;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_9;
    QRadioButton *MRI;
    QRadioButton *CT;
    QLabel *labelDE1_2;
    QLabel *labelDE1_3;
    QHBoxLayout *horizontalLayout_7;
    QSpinBox *DataX;
    QSpinBox *DataY;
    QSpinBox *DataZ;
    QHBoxLayout *horizontalLayout_2;
    QDoubleSpinBox *VoxelSize_x;
    QDoubleSpinBox *VoxelSize_y;
    QDoubleSpinBox *VoxelSize_z;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_4;
    QRadioButton *Short;
    QRadioButton *Double;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *DataDimension)
    {
        if (DataDimension->objectName().isEmpty())
            DataDimension->setObjectName(QString::fromUtf8("DataDimension"));
        DataDimension->resize(220, 250);
        DataDimension->setMinimumSize(QSize(220, 250));
        DataDimension->setMaximumSize(QSize(220, 250));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icon/icon/Screenshot.png"), QSize(), QIcon::Normal, QIcon::Off);
        DataDimension->setWindowIcon(icon);
        gridLayout_2 = new QGridLayout(DataDimension);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        SurfaceGroupBox = new QGroupBox(DataDimension);
        SurfaceGroupBox->setObjectName(QString::fromUtf8("SurfaceGroupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SurfaceGroupBox->sizePolicy().hasHeightForWidth());
        SurfaceGroupBox->setSizePolicy(sizePolicy);
        SurfaceGroupBox->setMinimumSize(QSize(200, 191));
        SurfaceGroupBox->setMaximumSize(QSize(200, 191));
        SurfaceGroupBox->setAutoFillBackground(true);
        SurfaceGroupBox->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(SurfaceGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_7 = new QGroupBox(SurfaceGroupBox);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        gridLayout_9 = new QGridLayout(groupBox_7);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        MRI = new QRadioButton(groupBox_7);
        MRI->setObjectName(QString::fromUtf8("MRI"));
        MRI->setCursor(QCursor(Qt::PointingHandCursor));
        MRI->setChecked(true);

        gridLayout_9->addWidget(MRI, 0, 0, 1, 1);

        CT = new QRadioButton(groupBox_7);
        CT->setObjectName(QString::fromUtf8("CT"));
        CT->setCursor(QCursor(Qt::PointingHandCursor));
        CT->setChecked(false);

        gridLayout_9->addWidget(CT, 0, 1, 1, 1);


        gridLayout->addWidget(groupBox_7, 0, 0, 1, 1);

        labelDE1_2 = new QLabel(SurfaceGroupBox);
        labelDE1_2->setObjectName(QString::fromUtf8("labelDE1_2"));
        labelDE1_2->setMaximumSize(QSize(176, 13));

        gridLayout->addWidget(labelDE1_2, 1, 0, 1, 1);

        labelDE1_3 = new QLabel(SurfaceGroupBox);
        labelDE1_3->setObjectName(QString::fromUtf8("labelDE1_3"));
        labelDE1_3->setMaximumSize(QSize(176, 13));

        gridLayout->addWidget(labelDE1_3, 3, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        DataX = new QSpinBox(SurfaceGroupBox);
        DataX->setObjectName(QString::fromUtf8("DataX"));
        DataX->setAlignment(Qt::AlignCenter);
        DataX->setButtonSymbols(QAbstractSpinBox::NoButtons);
        DataX->setMaximum(100000);

        horizontalLayout_7->addWidget(DataX);

        DataY = new QSpinBox(SurfaceGroupBox);
        DataY->setObjectName(QString::fromUtf8("DataY"));
        DataY->setAlignment(Qt::AlignCenter);
        DataY->setButtonSymbols(QAbstractSpinBox::NoButtons);
        DataY->setMaximum(100000);

        horizontalLayout_7->addWidget(DataY);

        DataZ = new QSpinBox(SurfaceGroupBox);
        DataZ->setObjectName(QString::fromUtf8("DataZ"));
        DataZ->setAlignment(Qt::AlignCenter);
        DataZ->setButtonSymbols(QAbstractSpinBox::NoButtons);
        DataZ->setMinimum(0);
        DataZ->setMaximum(100000);
        DataZ->setValue(0);

        horizontalLayout_7->addWidget(DataZ);


        gridLayout->addLayout(horizontalLayout_7, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        VoxelSize_x = new QDoubleSpinBox(SurfaceGroupBox);
        VoxelSize_x->setObjectName(QString::fromUtf8("VoxelSize_x"));
        VoxelSize_x->setMinimumSize(QSize(54, 23));
        VoxelSize_x->setMaximumSize(QSize(16777215, 16777215));
        VoxelSize_x->setAlignment(Qt::AlignCenter);
        VoxelSize_x->setButtonSymbols(QAbstractSpinBox::NoButtons);
        VoxelSize_x->setDecimals(5);

        horizontalLayout_2->addWidget(VoxelSize_x);

        VoxelSize_y = new QDoubleSpinBox(SurfaceGroupBox);
        VoxelSize_y->setObjectName(QString::fromUtf8("VoxelSize_y"));
        VoxelSize_y->setMinimumSize(QSize(54, 23));
        VoxelSize_y->setMaximumSize(QSize(16777215, 16777215));
        VoxelSize_y->setAlignment(Qt::AlignCenter);
        VoxelSize_y->setButtonSymbols(QAbstractSpinBox::NoButtons);
        VoxelSize_y->setDecimals(5);

        horizontalLayout_2->addWidget(VoxelSize_y);

        VoxelSize_z = new QDoubleSpinBox(SurfaceGroupBox);
        VoxelSize_z->setObjectName(QString::fromUtf8("VoxelSize_z"));
        VoxelSize_z->setMinimumSize(QSize(54, 23));
        VoxelSize_z->setMaximumSize(QSize(16777215, 16777215));
        VoxelSize_z->setAlignment(Qt::AlignCenter);
        VoxelSize_z->setButtonSymbols(QAbstractSpinBox::NoButtons);
        VoxelSize_z->setDecimals(5);

        horizontalLayout_2->addWidget(VoxelSize_z);


        gridLayout->addLayout(horizontalLayout_2, 4, 0, 1, 1);

        groupBox_8 = new QGroupBox(SurfaceGroupBox);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        gridLayout_4 = new QGridLayout(groupBox_8);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        Short = new QRadioButton(groupBox_8);
        Short->setObjectName(QString::fromUtf8("Short"));
        Short->setCursor(QCursor(Qt::PointingHandCursor));
        Short->setChecked(true);

        gridLayout_4->addWidget(Short, 0, 0, 1, 1);

        Double = new QRadioButton(groupBox_8);
        Double->setObjectName(QString::fromUtf8("Double"));
        Double->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout_4->addWidget(Double, 0, 1, 1, 1);


        gridLayout->addWidget(groupBox_8, 5, 0, 1, 1);


        gridLayout_2->addWidget(SurfaceGroupBox, 0, 0, 1, 1);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(DataDimension);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setCursor(QCursor(Qt::PointingHandCursor));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(DataDimension);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setCursor(QCursor(Qt::PointingHandCursor));

        hboxLayout->addWidget(cancelButton);


        gridLayout_2->addLayout(hboxLayout, 1, 0, 1, 1);


        retranslateUi(DataDimension);
        QObject::connect(okButton, SIGNAL(clicked()), DataDimension, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), DataDimension, SLOT(reject()));

        QMetaObject::connectSlotsByName(DataDimension);
    } // setupUi

    void retranslateUi(QDialog *DataDimension)
    {
        DataDimension->setWindowTitle(QApplication::translate("DataDimension", "DataDimension", 0, QApplication::UnicodeUTF8));
        SurfaceGroupBox->setTitle(QString());
        groupBox_7->setTitle(QString());
        MRI->setText(QApplication::translate("DataDimension", "MRI", 0, QApplication::UnicodeUTF8));
        CT->setText(QApplication::translate("DataDimension", "CT", 0, QApplication::UnicodeUTF8));
        labelDE1_2->setText(QApplication::translate("DataDimension", "Data extent", 0, QApplication::UnicodeUTF8));
        labelDE1_3->setText(QApplication::translate("DataDimension", "Voxel size", 0, QApplication::UnicodeUTF8));
        groupBox_8->setTitle(QString());
        Short->setText(QApplication::translate("DataDimension", "short", 0, QApplication::UnicodeUTF8));
        Double->setText(QApplication::translate("DataDimension", "double", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("DataDimension", "OK", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("DataDimension", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DataDimension: public Ui_DataDimension {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATADIMENSION_H
