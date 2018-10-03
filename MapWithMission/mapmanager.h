#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <QObject>
#include <QMetaType>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <chrono>
#include <functional>
#include <unistd.h>
#include "imagestitcher.h"
#include "baseimageinput.h"
#include "videoimageinput.h"
#include "cameraimageinput.h"
#include "filelistimageinput.h"
#include "ipcamimageinput.h"

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
    void threadFunction();

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
    map_callback callbackFunction;

    std::string fileList;
    std::string videoFile;
    std::string camIP;
    int cam_id;

signals:
    void publishFrames(cv::Mat&, cv::Mat&);
    void publishStates(std::string&);

private:
    State curState = {STOP};
    InputMethod method;
    bool opened = {false};
    bool finished= {false};
    float period = {1};
    float micro_seconds;

    const int frame_h= {360};
    const int frame_w= {640};

    std::thread _thread;

    cv::Mat curFrame;
    int curIndex = {0};

    std::shared_ptr<ImageStitcher> _stitcher;
    std::shared_ptr<BaseImageInput> _input;

    cv::Mat equalize(cv::Mat& image);
};

#endif // MAPMANAGER_H
