#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmessagebox.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _mapManager.reset(new MapManager());
    map_callback callback=std::bind(&MainWindow::drawImages,this,std::placeholders::_1,std::placeholders::_2);
    _mapManager->callbackFunction=callback;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_open_cam_clicked()
{
    _mapManager->cam_id=ui->combo_cam_id->currentText().toInt();
    _mapManager->open(CAMERA);
}

void MainWindow::on_btn_open_list_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Select file list"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter(tr("TXTs(*.txt)"));
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
        std::string fileName=fileNames.at(0).toStdString();
        _mapManager->fileList=fileName;
        _mapManager->open(FILELIST);
    }
    delete fileDialog;
}

void MainWindow::on_btn_open_video_clicked()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Select file list"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter(tr("Videos(*.avi *.mp4 *.flv)"));
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
        std::string videoFile=fileNames.at(0).toStdString();
        _mapManager->videoFile=videoFile;
        _mapManager->open(VIDEO);
    }
}

void MainWindow::on_btn_start_clicked()
{
    if(!_mapManager->isOpened()){
        QMessageBox::warning(this,tr("Warning"),tr("Input source not opened!"),QMessageBox::Ok);
        return;
    }
    updatePeriod();
    if(_mapManager->getCurState()==STOP){
        _mapManager->updateState(BUILD);
        _mapManager->start();
    }
    else{
        _mapManager->updateState(BUILD);
    }
}

void MainWindow::on_btn_pause_clicked()
{
    if(!_mapManager->isOpened()){
        QMessageBox::warning(this,tr("Warning"),tr("Input source not opened!"),QMessageBox::Ok);
        return;
    }
    _mapManager->updateState(PAUSE);
}

void MainWindow::on_btn_resume_clicked()
{
    if(!_mapManager->isOpened()){
        QMessageBox::warning(this,tr("Warning"),tr("Input source not opened!"),QMessageBox::Ok);
        return;
    }
    _mapManager->updateState(BUILD);
}

void MainWindow::on_btn_finish_clicked()
{
    if(!_mapManager->isOpened()){
        QMessageBox::warning(this,tr("Warning"),tr("Input source not opened!"),QMessageBox::Ok);
        return;
    }
    _mapManager->updateState(STOP);
}

void MainWindow::on_btn_save_clicked()
{
    cv::Mat map2d=_mapManager->getImage();
    std::stringstream ss;
    ss<<"./maps/map_"<<savedIndex<<".jpg";
    std::cout<<"Savint to "<<ss.str()<<std::endl;
    savedIndex++;
    cv::imwrite(ss.str(), map2d);
}

void MainWindow::on_btn_update_period_clicked()
{
    updatePeriod();
}

void MainWindow::drawImages(cv::Mat& map, cv::Mat& curFrame){
    if(curFrame.empty()){
        QMessageBox::information(this,tr("Info"),tr("All frames stitched!"),QMessageBox::Ok);
        return;
    }
    int map_w=ui->label_map->width();
    int map_h=ui->label_map->height();
    int frame_w=ui->label_frame->width();
    int frame_h=ui->label_frame->height();

    if(_mapManager->getCurState()==BUILD){
        float ratio=MIN(frame_h*1.0/curFrame.rows, frame_w*1.0/curFrame.cols);
        cv::resize(curFrame, curFrame, cv::Size(0,0), ratio, ratio, cv::INTER_LINEAR);
        ratio=MIN(map_h*1.0/map.rows, map_w*1.0/map.cols);
        cv::resize(map, map, cv::Size(0,0), ratio, ratio, cv::INTER_LINEAR);
        cv::Mat canvas=cv::Mat::zeros(map_h, map_w, CV_8UC3);
        int start_y=MAX(0,std::floor((map_h-map.rows)/2));
        int start_x=MAX(0,std::floor((map_w-map.cols)/2));
        cv::Mat img_part=map(cv::Rect(0,0,MIN(map.cols, map_w), MIN(map.rows, map_h)));
        img_part.copyTo(canvas(cv::Rect(start_x, start_y, MIN(map.cols, map_w), MIN(map.rows, map_h))));

        //cv::cvtColor(canvas, canvas, cv::COLOR_BGR2RGB);
        cv::cvtColor(map, map, cv::COLOR_BGR2RGB);
        cv::cvtColor(curFrame, curFrame, cv::COLOR_BGR2RGB);
        //QImage q_map = QImage((const unsigned char*)(canvas.data), canvas.cols, canvas.rows, canvas.cols*canvas.channels(), QImage::Format_RGB888);
        QImage q_map = QImage((const unsigned char*)(map.data), map.cols, map.rows, map.cols*map.channels(), QImage::Format_RGB888);
        QImage q_frame=QImage((const unsigned char*)(curFrame.data), curFrame.cols, curFrame.rows, curFrame.cols*curFrame.channels(), QImage::Format_RGB888);

        ui->label_map->setPixmap(QPixmap::fromImage(q_map));
        ui->label_frame->setPixmap(QPixmap::fromImage(q_frame));

    }
    else if(_mapManager->getCurState()==PREVIEW){
        float ratio=MIN(map_h*1.0/curFrame.rows, map_w*1.0/curFrame.cols);
        cv::resize(curFrame, curFrame, cv::Size(0,0), ratio, ratio, cv::INTER_LINEAR);
        cv::cvtColor(curFrame, curFrame, cv::COLOR_BGR2RGB);
        QImage q_frame=QImage((const unsigned char*)(curFrame.data), curFrame.cols, curFrame.rows, curFrame.cols*curFrame.channels(), QImage::Format_RGB888);
        ui->label_map->setPixmap(QPixmap::fromImage(q_frame));
    }
    if(_mapManager->isFinished()){
        QMessageBox::information(this,tr("Info"),tr("All frames stitched!"),QMessageBox::Ok);
    }
}

void MainWindow::updatePeriod(){
    float max_value=ui->txt_max_time->text().toFloat();
    float period=1.0*ui->slide_period->value()/ui->slide_period->maximum()*max_value;
    _mapManager->updatePeriod(period);
    ui->txt_cur_time->setText(QString::number(period));
}

void MainWindow::on_slide_period_actionTriggered(int action)
{
    updatePeriod();
}

void MainWindow::on_btn_preview_clicked()
{
    if(!_mapManager->isOpened()){
        QMessageBox::warning(this,tr("Warning"),tr("Input source not opened!"),QMessageBox::Ok);
        return;
    }
    updatePeriod();
    if(_mapManager->getCurState()==STOP){
        _mapManager->updateState(PREVIEW);
        _mapManager->start();
    }
    else{
        _mapManager->updateState(PREVIEW);
    }
}

void MainWindow::on_slide_offset_actionTriggered(int action)
{
    float max_value=ui->slide_offset->maximum();
    float alpha=1.0*ui->slide_offset->value()/max_value;
    _mapManager->updateStitchAlphas(alpha, "OFFSET");
}

void MainWindow::on_slide_optim_actionTriggered(int action)
{
    float max_value=ui->slide_optim->maximum();
    float alpha=1.0*ui->slide_optim->value()/max_value;
    _mapManager->updateStitchAlphas(alpha, "OPTIM");
}
