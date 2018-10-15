#ifndef BASEIMAGEINPUT_H
#define BASEIMAGEINPUT_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include "cameracalibration.h"

class BaseImageInput
{
public:
    BaseImageInput(int d);
    BaseImageInput(const std::string& s);

    inline int getNumFrames(){
        return numFrames;
    }
    inline int getCurFrameIndex(){
        return curFrameIndex;
    }
    inline bool isFrameFinite(){
        return isFinite;
    }
    inline bool isOpened(){
        return opened;
    }

    inline void startRecord(const std::string& video_name){
        writer.open(video_name, CV_FOURCC('M','J','P','G'), 30, cv::Size(im_w, im_h));
        bRecording=(writer.isOpened());
    }

    inline void endRecord(){
        bRecording=false;
    }
    virtual cv::Mat getRawImage()=0;

    virtual void release(){}

    virtual void stop(){}

    int im_w = {0};
    int im_h = {0};

protected:
    int numFrames;
    int curFrameIndex ={0};
    bool isFinite;
    bool opened;
    bool calib = {false};
    bool bRecording = {false};


    int digitSource;
    std::string strSource;

    cv::VideoWriter writer;
    CameraCalibration cam;
};

#endif // BASEIMAGEINPUT_H
