#ifndef MAPMANAGER_H
#define MAPMANAGER_H

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

enum InputMethod{
    CAMERA,
    VIDEO,
    FILELIST,
    FOLDER
};

enum State{
    BUILD,
    PAUSE,
    STOP
};

using map_callback=std::function<void(cv::Mat&, cv::Mat&)>;

class MapManager
{
public:
    MapManager();

    void open(InputMethod method);
    void start();
    void threadFunction();

    cv::Mat getImage();

    inline void updateState(State s){
        curState=s;
    }
    inline bool isOpened(){
        return opened;
    }
    inline void updatePeriod(float p){
        period=p;
    }
    inline bool isFinished(){
        //if(!_input->isFrameFinite())
        //    return false;
        //return (_input->getCurFrameIndex()==_input->getNumFrames());
        return finished;
    }

    map_callback callbackFunction;

    std::string fileList;
    std::string videoFile;
    int cam_id;

private:
    State curState = {STOP};
    InputMethod method;
    bool opened = {false};
    bool finished= {false};
    float period = {1};

    const int frame_h= {360};
    const int frame_w= {640};

    std::thread _thread;

    cv::Mat curFrame;
    int curIndex = {0};

    std::shared_ptr<ImageStitcher> _stitcher;
    std::shared_ptr<BaseImageInput> _input;
};

#endif // MAPMANAGER_H
