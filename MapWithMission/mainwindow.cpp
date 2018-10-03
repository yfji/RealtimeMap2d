#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmessagebox.h>
#include <QFileDialog>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _mapManager.reset(new MapManager());
    _gpsClient.reset(new GPSClient());

    map_callback callback=std::bind(&MainWindow::drawImages,this,std::placeholders::_1,std::placeholders::_2);
    _mapManager->callbackFunction=callback;
    _mapLabel=ui->label_map;

    QObject::connect(_mapManager.get(), SIGNAL(publishFrames(cv::Mat&,cv::Mat&)), SLOT(onUpdateFrames(cv::Mat&,cv::Mat&)));
    QObject::connect(_mapManager.get(), SIGNAL(publishStates(std::string&)), SLOT(onUpdateStates(std::string&)));
    QObject::connect(_mapLabel, SIGNAL(publishClickedPoint(cv::Point2f&)), SLOT(onUpdateClickedPoint(cv::Point2f&)));
    QObject::connect(&(_gpsClient->publisher), SIGNAL(publishGPSMessage(std::string)), SLOT(onGPSUpdate(std::string)));

    _gpsClient->startConnect(10);
}

MainWindow::~MainWindow()
{
    _gpsClient->stopConnect();
    usleep(30e3);
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


void MainWindow::on_btn_open_ipcam_clicked()
{
    std::string ipAddr=ui->txt_ip_addr->text().toStdString();
    std::cout<<ipAddr<<std::endl;
    _mapManager->camIP=ipAddr;
    _mapManager->open(IPCAMERA);
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
    if(_mapManager->getCurState()==BUILD)
        _mapManager->updateState(PAUSE);
}

void MainWindow::on_btn_resume_clicked()
{
    if(!_mapManager->isOpened()){
        QMessageBox::warning(this,tr("Warning"),tr("Input source not opened!"),QMessageBox::Ok);
        return;
    }
    if(_mapManager->getCurState()==PAUSE)
        _mapManager->updateState(BUILD);
}

void MainWindow::on_btn_finish_clicked()
{
    if(!_mapManager->isOpened()){
        QMessageBox::warning(this,tr("Warning"),tr("Input source not opened!"),QMessageBox::Ok);
        return;
    }
    _mapManager->updateState(STOP);
    QMessageBox::information(this,tr("Info"),tr("All frames stitched!"),QMessageBox::Ok);
}

void MainWindow::on_btn_save_clicked()
{
    cv::Mat map2d=_mapManager->getImage(false);
    std::stringstream ss;
    ss<<"./maps/map_"<<savedIndex<<".jpg";
    std::cout<<"Savint to "<<ss.str()<<std::endl;
    savedIndex++;
    cv::imwrite(ss.str(), map2d);
}

void MainWindow::drawImages(cv::Mat& map, cv::Mat& curFrame){
    if(curFrame.empty()){
        //QMessageBox::information(this,tr("Info"),tr("All frames stitched!"),QMessageBox::Ok);
        return;
    }
    int map_w=ui->label_map->width();
    int map_h=ui->label_map->height();
    int frame_w=ui->label_frame->width();
    int frame_h=ui->label_frame->height();

    if(_mapManager->getCurState()==BUILD || _mapManager->getCurState()==PAUSE){
        float ratio=MIN(frame_h*1.0/curFrame.rows, frame_w*1.0/curFrame.cols);
        cv::resize(curFrame, curFrame, cv::Size(0,0), ratio, ratio, cv::INTER_LINEAR);
        ratio=MIN(map_h*1.0/map.rows, map_w*1.0/map.cols);
        cv::resize(map, map, cv::Size(0,0), ratio, ratio, cv::INTER_LINEAR);
        _mapScale=ratio;

        //cv::Mat canvas=cv::Mat::zeros(map_h, map_w, CV_8UC3);
        //int start_y=MAX(0,std::floor((map_h-map.rows)/2));
        //int start_x=MAX(0,std::floor((map_w-map.cols)/2));
        //cv::Mat img_part=map(cv::Rect(0,0,MIN(map.cols, map_w), MIN(map.rows, map_h)));
        //img_part.copyTo(canvas(cv::Rect(start_x, start_y, MIN(map.cols, map_w), MIN(map.rows, map_h))));
        //cv::cvtColor(canvas, canvas, cv::COLOR_BGR2RGB);

        cv::Mat tmp1, tmp2;
        cv::cvtColor(map, tmp1, cv::COLOR_BGR2RGB);
        cv::cvtColor(curFrame, tmp2, cv::COLOR_BGR2RGB);
        //QImage q_map = QImage((const unsigned char*)(canvas.data), canvas.cols, canvas.rows, canvas.cols*canvas.channels(), QImage::Format_RGB888);
        QImage q_map = QImage((const unsigned char*)(tmp1.data), tmp1.cols, tmp1.rows, tmp1.cols*tmp1.channels(), QImage::Format_RGB888);
        QImage q_frame=QImage((const unsigned char*)(tmp2.data), tmp2.cols, tmp2.rows, tmp2.cols*tmp2.channels(), QImage::Format_RGB888);

        ui->label_map->setPixmap(QPixmap::fromImage(q_map));
        ui->label_frame->setPixmap(QPixmap::fromImage(q_frame));

    }
    else if(_mapManager->getCurState()==PREVIEW){
        float ratio=MIN(map_h*1.0/curFrame.rows, map_w*1.0/curFrame.cols);
        _mapScale=ratio;
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
    ui->txt_offset->setText(QString::number(alpha));
    _mapManager->updateStitchAlphas(alpha, "OFFSET");
}

void MainWindow::on_slide_optim_actionTriggered(int action)
{
    float max_value=ui->slide_optim->maximum();
    float alpha=1.0*ui->slide_optim->value()/max_value;
    ui->txt_optim->setText(QString::number(alpha));
    _mapManager->updateStitchAlphas(alpha, "OPTIM");
}

void MainWindow::on_check_orb_stateChanged(int arg1)
{

}

void MainWindow::onUpdateFrames(cv::Mat & map2d, cv::Mat & curFrame){
    drawImages(map2d, curFrame);
}

void MainWindow::onUpdateStates(std::string & s){
    QString qstr=ui->txt_console->toPlainText();
    if(qstr.length()==0)
        qstr=tr(s.c_str());
    else
        qstr=qstr+tr("\n")+tr(s.c_str());
    ui->txt_console->setText(qstr);
    consoleLines++;
    if(consoleLines==500){
        ui->txt_console->clear();
        consoleLines=0;
    }
    QScrollBar *scrollbar = ui->txt_console->verticalScrollBar();
    if (scrollbar)
    {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}
void MainWindow::onUpdateClickedPoint(cv::Point2f & point){
    if(_mapManager->getCurState()==BUILD){
        point.x/=_mapScale;
        point.y/=_mapScale;
        _mapManager->updateClickedPoint(point);
        std::string str="Update match center manually";
        onUpdateStates(str);
    }
}

void MainWindow::onGPSUpdate(std::string msg){
    decodeGPSMessage(msg);
    char data_lat[20], data_lon[20];
    int count=(int)gpsDataMap["count"];

    float lat=gpsDataMap["lat"];
    float lon=gpsDataMap["lon"];

    sprintf(data_lat, "%.2f", lat);
    sprintf(data_lon, "%.2f", lon);
    ui->txt_count->setText(QString::number(count));
    ui->txt_lat->setText(QString(data_lat));
    ui->txt_lon->setText(QString(data_lon));

    _mapManager->gps=GPS{lat, lon};
}

void MainWindow::on_slide_ignore_actionTriggered(int action)
{
    float max_value=ui->slide_ignore->maximum();
    float value=1.0*ui->slide_ignore->value()/max_value;
    ui->txt_ignore->setText(QString::number(value));
    _mapManager->updateIgnoreThreshold(value);
}

void MainWindow::decodeGPSMessage(const std::string &msg){
    std::vector<std::string> items;
    char item[100];
    int item_index=0;
    std::map<std::string, float> gpsData;

    for(int i=0;i<msg.length();++i){
        item[item_index++]=msg[i];
        if(msg[i]==',' || i==msg.length()-1){
            item[item_index-1]='\0';
            items.push_back(std::string(item));
            item_index=0;
        }
    }
    for(int i=0;i<items.size();++i){
        std::string str_item=items[i];
        int pos=str_item.find(':');
        std::stringstream ss;
        for(int j=0;j<str_item.length();++j){
            if(j!=pos){
                ss<<str_item[j];
            }
            else{
                ss<<' ';
            }
        }
        std::string key;
        float val;
        ss>>key>>val;
        gpsData.insert(std::pair<std::string, float>(key, val));
    }
    gpsDataMap=gpsData;
}
