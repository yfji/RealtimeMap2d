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
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QLabel *label_map;
    QGroupBox *groupBox_2;
    QPushButton *btn_open_cam;
    QPushButton *btn_open_video;
    QPushButton *btn_open_list;
    QLabel *label_3;
    QComboBox *combo_cam_id;
    QGroupBox *groupBox_3;
    QPushButton *btn_start;
    QPushButton *btn_pause;
    QPushButton *btn_resume;
    QPushButton *btn_finish;
    QPushButton *btn_save;
    QLabel *label_2;
    QPushButton *btn_update_period;
    QSlider *slide_period;
    QLabel *label;
    QLineEdit *txt_max_time;
    QLabel *txt_cur_time;
    QGroupBox *groupBox_4;
    QLabel *label_frame;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1107, 809);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 671, 721));
        label_map = new QLabel(groupBox);
        label_map->setObjectName(QStringLiteral("label_map"));
        label_map->setGeometry(QRect(10, 30, 640, 640));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(710, 0, 351, 131));
        btn_open_cam = new QPushButton(groupBox_2);
        btn_open_cam->setObjectName(QStringLiteral("btn_open_cam"));
        btn_open_cam->setGeometry(QRect(20, 30, 101, 51));
        btn_open_video = new QPushButton(groupBox_2);
        btn_open_video->setObjectName(QStringLiteral("btn_open_video"));
        btn_open_video->setGeometry(QRect(240, 30, 101, 51));
        btn_open_list = new QPushButton(groupBox_2);
        btn_open_list->setObjectName(QStringLiteral("btn_open_list"));
        btn_open_list->setGeometry(QRect(130, 30, 101, 51));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 90, 101, 31));
        combo_cam_id = new QComboBox(groupBox_2);
        combo_cam_id->addItem(QString());
        combo_cam_id->addItem(QString());
        combo_cam_id->addItem(QString());
        combo_cam_id->setObjectName(QStringLiteral("combo_cam_id"));
        combo_cam_id->setGeometry(QRect(110, 90, 121, 31));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(710, 140, 351, 241));
        groupBox_3->setInputMethodHints(Qt::ImhNone);
        btn_start = new QPushButton(groupBox_3);
        btn_start->setObjectName(QStringLiteral("btn_start"));
        btn_start->setGeometry(QRect(30, 30, 141, 91));
        btn_pause = new QPushButton(groupBox_3);
        btn_pause->setObjectName(QStringLiteral("btn_pause"));
        btn_pause->setGeometry(QRect(190, 30, 131, 41));
        btn_resume = new QPushButton(groupBox_3);
        btn_resume->setObjectName(QStringLiteral("btn_resume"));
        btn_resume->setGeometry(QRect(190, 80, 131, 41));
        btn_finish = new QPushButton(groupBox_3);
        btn_finish->setObjectName(QStringLiteral("btn_finish"));
        btn_finish->setGeometry(QRect(30, 140, 141, 41));
        btn_save = new QPushButton(groupBox_3);
        btn_save->setObjectName(QStringLiteral("btn_save"));
        btn_save->setGeometry(QRect(190, 140, 131, 41));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(200, 200, 16, 31));
        btn_update_period = new QPushButton(groupBox_3);
        btn_update_period->setObjectName(QStringLiteral("btn_update_period"));
        btn_update_period->setGeometry(QRect(220, 200, 101, 31));
        slide_period = new QSlider(groupBox_3);
        slide_period->setObjectName(QStringLiteral("slide_period"));
        slide_period->setGeometry(QRect(30, 220, 161, 16));
        slide_period->setOrientation(Qt::Horizontal);
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 190, 16, 21));
        txt_max_time = new QLineEdit(groupBox_3);
        txt_max_time->setObjectName(QStringLiteral("txt_max_time"));
        txt_max_time->setGeometry(QRect(140, 190, 51, 25));
        txt_max_time->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        txt_cur_time = new QLabel(groupBox_3);
        txt_cur_time->setObjectName(QStringLiteral("txt_cur_time"));
        txt_cur_time->setGeometry(QRect(60, 190, 61, 21));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(710, 390, 351, 331));
        label_frame = new QLabel(groupBox_4);
        label_frame->setObjectName(QStringLiteral("label_frame"));
        label_frame->setGeometry(QRect(20, 30, 311, 280));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1107, 28));
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

        groupBox_3->setTitle(QApplication::translate("MainWindow", "Functions", nullptr));
        btn_start->setText(QApplication::translate("MainWindow", "Start  Build", nullptr));
        btn_pause->setText(QApplication::translate("MainWindow", "Pause", nullptr));
        btn_resume->setText(QApplication::translate("MainWindow", "Resume", nullptr));
        btn_finish->setText(QApplication::translate("MainWindow", "Finish Build", nullptr));
        btn_save->setText(QApplication::translate("MainWindow", "Save Map", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "s", nullptr));
        btn_update_period->setText(QApplication::translate("MainWindow", "Update", nullptr));
        label->setText(QApplication::translate("MainWindow", "0", nullptr));
        txt_max_time->setText(QApplication::translate("MainWindow", "1.0", nullptr));
        txt_cur_time->setText(QString());
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Current Frame", nullptr));
        label_frame->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
