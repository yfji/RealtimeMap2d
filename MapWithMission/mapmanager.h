#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <QObject>
#include <QMetaType>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>
#include <atomic>
#include <chrono>
#include <functional>
#include <unistd.h>
#include "imagestitcher.h"
#include "baseimageinput.h"
#include "videoimageinput.h"
#include "cameraimageinput.h"
#include "filelistimageinput.h"
#include "ipcamimageinput.h"
#include "mission.h"

enum InputMethod{
    CAMERA,
    IPCAMERA,
    VIDEO,
    FILELIST,
    FOLDER
};

enum State{
    PREVIEW,
    BUILD,
    PAUSE,
    STOP
};

using map_callback=std::function<void(cv::Mat&, cv::Mat&)>;

class MapManager :public QObject
{
    Q_OBJECT
public:
    explicit MapManager(QObject* parent=0);

    ~MapManager(){}

    void open(InputMethod method);
    void start();

    cv::Mat getImage(bool withRect=true);

    inline void updateState(State s){
        curState=s;
    }
    inline bool isOpened(){
        return opened;
    }
    inline void updatePeriod(float p){
        period=p;
    }
    inline void updateClickedPoint(cv::Point2f& point){
        _stitcher->updateManual(point);
    }
    inline bool isFinished(){
        //if(!_input->isFrameFinite())
        //    return false;
        //return (_input->getCurFrameIndex()==_input->getNumFrames());
        return finished;
    }
    inline void updateStitchAlphas(float _a, const char* name){
        if(strcmp(name, "OFFSET")==0){
            _stitcher->updateAlphaOffset(_a);
        }
        else if(strcmp(name, "OPTIM")==0){
            _stitcher->updateAlphaOptim(_a);
        }
    }
    inline void updateIgnoreThreshold(float thresh){
        _stitcher->updateIgnoreThresh(thresh);
    }
    State getCurState(){
        return curState;
    }

    void record(bool flag);
    void detect(bool flag);

    map_callback callbackFunction;

    std::string fileList;
    std::string videoFile;
    std::string camIP;
    int cam_id;

    GPS gps ={30,16};

signals:
    void publishFrames(cv::Mat&, cv::Mat&);
    void publishStates(std::string&);

private:
    State curState = {STOP};
    InputMethod method;
    bool opened = {false};
    bool detecting = {false};
    bool finished= {false};
    bool use_image_thread ={false};
    float period = {1};
    float micro_seconds;

    int frame_index= {0};
    const int check_interval= {4};

    const int frame_h= {360};
    const int frame_w= {640};
    //const int frame_h= {426};
    //const int frame_w= {640};

    std::thread _thread;
    std::thread _image_thread;
    std::thread _mission_thread;

    std::atomic_bool _mission_run ={false};
    std::mutex img_mutex;

    cv::Mat curFrame;
    int curIndex = {0};
    int videoIndex= {0};

    std::shared_ptr<ImageStitcher> _stitcher;
    std::shared_ptr<BaseImageInput> _input;
    std::shared_ptr<Mission> _mission;

    cv::Mat equalize(cv::Mat& image);

    void getImageFunction();
    void threadFunction();
    void missionFunction();

};

#endif // MAPMANAGER_H
