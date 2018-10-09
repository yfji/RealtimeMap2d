/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "maplabel.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    MapLabel *label_map;
    QGroupBox *groupBox_2;
    QPushButton *btn_open_cam;
    QPushButton *btn_open_video;
    QPushButton *btn_open_list;
    QLabel *label_3;
    QComboBox *combo_cam_id;
    QPushButton *btn_open_ipcam;
    QLabel *label_7;
    QLineEdit *txt_ip_addr;
    QGroupBox *groupBox_3;
    QPushButton *btn_start;
    QPushButton *btn_pause;
    QPushButton *btn_resume;
    QPushButton *btn_finish;
    QPushButton *btn_save;
    QLabel *label_2;
    QSlider *slide_period;
    QLabel *label;
    QLineEdit *txt_max_time;
    QLabel *txt_cur_time;
    QSlider *slide_offset;
    QSlider *slide_optim;
    QLabel *txt_offset;
    QLabel *txt_optim;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QPushButton *btn_preview;
    QTextEdit *txt_console;
    QLineEdit *txt_count;
    QLineEdit *txt_lon;
    QLineEdit *txt_lat;
    QLabel *label_8;
    QSlider *slide_ignore;
    QLabel *txt_ignore;
    QGroupBox *groupBox_4;
    QLabel *label_frame;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1323, 848);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 751, 781));
        label_map = new MapLabel(groupBox);
        label_map->setObjectName(QStringLiteral("label_map"));
        label_map->setGeometry(QRect(10, 30, 731, 741));
        label_map->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(790, 0, 511, 131));
        btn_open_cam = new QPushButton(groupBox_2);
        btn_open_cam->setObjectName(QStringLiteral("btn_open_cam"));
        btn_open_cam->setGeometry(QRect(10, 30, 111, 51));
        btn_open_video = new QPushButton(groupBox_2);
        btn_open_video->setObjectName(QStringLiteral("btn_open_video"));
        btn_open_video->setGeometry(QRect(370, 30, 111, 51));
        btn_open_list = new QPushButton(groupBox_2);
        btn_open_list->setObjectName(QStringLiteral("btn_open_list"));
        btn_open_list->setGeometry(QRect(130, 30, 111, 51));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 90, 101, 31));
        combo_cam_id = new QComboBox(groupBox_2);
        combo_cam_id->addItem(QString());
        combo_cam_id->addItem(QString());
        combo_cam_id->addItem(QString());
        combo_cam_id->setObjectName(QStringLiteral("combo_cam_id"));
        combo_cam_id->setGeometry(QRect(110, 90, 51, 31));
        btn_open_ipcam = new QPushButton(groupBox_2);
        btn_open_ipcam->setObjectName(QStringLiteral("btn_open_ipcam"));
        btn_open_ipcam->setGeometry(QRect(250, 30, 111, 51));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(180, 96, 81, 21));
        txt_ip_addr = new QLineEdit(groupBox_2);
        txt_ip_addr->setObjectName(QStringLiteral("txt_ip_addr"));
        txt_ip_addr->setGeometry(QRect(260, 90, 221, 31));
        txt_ip_addr->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(790, 140, 511, 311));
        groupBox_3->setInputMethodHints(Qt::ImhNone);
        btn_start = new QPushButton(groupBox_3);
        btn_start->setObjectName(QStringLiteral("btn_start"));
        btn_start->setGeometry(QRect(20, 80, 141, 41));
        btn_pause = new QPushButton(groupBox_3);
        btn_pause->setObjectName(QStringLiteral("btn_pause"));
        btn_pause->setGeometry(QRect(180, 30, 131, 41));
        btn_resume = new QPushButton(groupBox_3);
        btn_resume->setObjectName(QStringLiteral("btn_resume"));
        btn_resume->setGeometry(QRect(180, 80, 131, 41));
        btn_finish = new QPushButton(groupBox_3);
        btn_finish->setObjectName(QStringLiteral("btn_finish"));
        btn_finish->setGeometry(QRect(20, 130, 141, 41));
        btn_save = new QPushButton(groupBox_3);
        btn_save->setObjectName(QStringLiteral("btn_save"));
        btn_save->setGeometry(QRect(180, 130, 131, 41));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(290, 190, 20, 21));
        slide_period = new QSlider(groupBox_3);
        slide_period->setObjectName(QStringLiteral("slide_period"));
        slide_period->setGeometry(QRect(80, 210, 191, 20));
        slide_period->setMaximum(100);
        slide_period->setValue(10);
        slide_period->setOrientation(Qt::Horizontal);
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(80, 190, 16, 21));
        txt_max_time = new QLineEdit(groupBox_3);
        txt_max_time->setObjectName(QStringLiteral("txt_max_time"));
        txt_max_time->setGeometry(QRect(230, 190, 51, 21));
        txt_max_time->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        txt_cur_time = new QLabel(groupBox_3);
        txt_cur_time->setObjectName(QStringLiteral("txt_cur_time"));
        txt_cur_time->setGeometry(QRect(130, 190, 61, 21));
        slide_offset = new QSlider(groupBox_3);
        slide_offset->setObjectName(QStringLiteral("slide_offset"));
        slide_offset->setGeometry(QRect(80, 230, 191, 20));
        slide_offset->setMaximum(100);
        slide_offset->setValue(30);
        slide_offset->setOrientation(Qt::Horizontal);
        slide_optim = new QSlider(groupBox_3);
        slide_optim->setObjectName(QStringLiteral("slide_optim"));
        slide_optim->setGeometry(QRect(80, 250, 191, 20));
        slide_optim->setMaximum(100);
        slide_optim->setValue(60);
        slide_optim->setOrientation(Qt::Horizontal);
        txt_offset = new QLabel(groupBox_3);
        txt_offset->setObjectName(QStringLiteral("txt_offset"));
        txt_offset->setGeometry(QRect(280, 230, 41, 17));
        txt_optim = new QLabel(groupBox_3);
        txt_optim->setObjectName(QStringLiteral("txt_optim"));
        txt_optim->setGeometry(QRect(280, 250, 41, 17));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 210, 67, 17));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 230, 67, 17));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 250, 67, 17));
        btn_preview = new QPushButton(groupBox_3);
        btn_preview->setObjectName(QStringLiteral("btn_preview"));
        btn_preview->setGeometry(QRect(20, 30, 141, 41));
        txt_console = new QTextEdit(groupBox_3);
        txt_console->setObjectName(QStringLiteral("txt_console"));
        txt_console->setGeometry(QRect(330, 80, 171, 221));
        txt_console->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        txt_console->setReadOnly(true);
        txt_count = new QLineEdit(groupBox_3);
        txt_count->setObjectName(QStringLiteral("txt_count"));
        txt_count->setGeometry(QRect(330, 30, 51, 41));
        QFont font;
        font.setPointSize(13);
        txt_count->setFont(font);
        txt_count->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        txt_count->setReadOnly(true);
        txt_lon = new QLineEdit(groupBox_3);
        txt_lon->setObjectName(QStringLiteral("txt_lon"));
        txt_lon->setGeometry(QRect(390, 30, 51, 41));
        txt_lon->setFont(font);
        txt_lon->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        txt_lon->setReadOnly(true);
        txt_lat = new QLineEdit(groupBox_3);
        txt_lat->setObjectName(QStringLiteral("txt_lat"));
        txt_lat->setGeometry(QRect(450, 30, 51, 41));
        txt_lat->setFont(font);
        txt_lat->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        txt_lat->setReadOnly(true);
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 270, 67, 17));
        slide_ignore = new QSlider(groupBox_3);
        slide_ignore->setObjectName(QStringLiteral("slide_ignore"));
        slide_ignore->setGeometry(QRect(80, 270, 191, 16));
        slide_ignore->setValue(33);
        slide_ignore->setOrientation(Qt::Horizontal);
        txt_ignore = new QLabel(groupBox_3);
        txt_ignore->setObjectName(QStringLiteral("txt_ignore"));
        txt_ignore->setGeometry(QRect(280, 270, 41, 16));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(790, 460, 511, 321));
        label_frame = new QLabel(groupBox_4);
        label_frame->setObjectName(QStringLiteral("label_frame"));
        label_frame->setGeometry(QRect(50, 30, 401, 281));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1323, 28));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        mainToolBar->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "Map", nullptr));
        label_map->setText(QString());
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Choose", nullptr));
        btn_open_cam->setText(QApplication::translate("MainWindow", "Open \n"
"Camera", nullptr));
        btn_open_video->setText(QApplication::translate("MainWindow", "Open\n"
"Video", nullptr));
        btn_open_list->setText(QApplication::translate("MainWindow", "Open\n"
"File List", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "camera_id", nullptr));
        combo_cam_id->setItemText(0, QApplication::translate("MainWindow", "0", nullptr));
        combo_cam_id->setItemText(1, QApplication::translate("MainWindow", "1", nullptr));
        combo_cam_id->setItemText(2, QApplication::translate("MainWindow", "2", nullptr));

        btn_open_ipcam->setText(QApplication::translate("MainWindow", "Open\n"
"IP Camera", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "IP Address", nullptr));
        txt_ip_addr->setText(QApplication::translate("MainWindow", "rtsp://192.168.2.220:554/stream/0", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Functions", nullptr));
        btn_start->setText(QApplication::translate("MainWindow", "Start  Build", nullptr));
        btn_pause->setText(QApplication::translate("MainWindow", "Pause", nullptr));
        btn_resume->setText(QApplication::translate("MainWindow", "Resume", nullptr));
        btn_finish->setText(QApplication::translate("MainWindow", "Finish Build", nullptr));
        btn_save->setText(QApplication::translate("MainWindow", "Save Map", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "s", nullptr));
        label->setText(QApplication::translate("MainWindow", "0", nullptr));
        txt_max_time->setText(QApplication::translate("MainWindow", "1.0", nullptr));
        txt_cur_time->setText(QString());
        txt_offset->setText(QString());
        txt_optim->setText(QString());
        label_4->setText(QApplication::translate("MainWindow", "period", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "offset", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "optimize", nullptr));
        btn_preview->setText(QApplication::translate("MainWindow", "Start Preview", nullptr));
        txt_count->setText(QApplication::translate("MainWindow", "0", nullptr));
        txt_lon->setText(QApplication::translate("MainWindow", "0", nullptr));
        txt_lat->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "ignore", nullptr));
        txt_ignore->setText(QString());
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Current Frame", nullptr));
        label_frame->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
