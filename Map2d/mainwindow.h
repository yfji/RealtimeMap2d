#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mapmanager.h"
#include "maplabel.h"
#include <memory>

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
    void on_btn_open_cam_clicked();

    void on_btn_open_list_clicked();

    void on_btn_start_clicked();

    void on_btn_pause_clicked();

    void on_btn_resume_clicked();

    void on_btn_finish_clicked();

    void on_btn_save_clicked();

    void on_btn_open_video_clicked();

    void on_slide_period_actionTriggered(int action);

    void updatePeriod();

    void on_btn_preview_clicked();

    void on_slide_offset_actionTriggered(int action);

    void on_slide_optim_actionTriggered(int action);

    void on_check_orb_stateChanged(int arg1);

    void onUpdateFrames(cv::Mat&, cv::Mat&);
    void onUpdateStates(std::string&);
    void onUpdateClickedPoint(cv::Point2f&);

    void on_btn_open_ipcam_clicked();

    void on_slide_ignore_actionTriggered(int action);

private:
    Ui::MainWindow *ui;
    bool _requestStop = {false};
    std::shared_ptr<MapManager> _mapManager;
    MapLabel* _mapLabel;

    //map_callback callback;
    int savedIndex= {0};
    int consoleLines={0};
    float _mapScale= {1.};
    bool _inputSourceSelected= {false};

    void drawImages(cv::Mat& map, cv::Mat& curFrame);
};

#endif // MAINWINDOW_H
