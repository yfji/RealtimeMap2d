#ifndef BASEIMAGEINPUT_H
#define BASEIMAGEINPUT_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include "mission.h"
#include "cameracalibration.h"

enum InputMethod{
    CAMERA,
    IPCAMERA,
    VIDEO,
    FILELIST,
    FOLDER
};

#define SAVE_GPS

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

    void startRecord();

    inline void endRecord(){
        bRecording=false;
    }
    virtual cv::Mat getRawImage()=0;

    virtual std::vector<float> getGPS(){
        return {0,0};
    }

    virtual void release(){}

    virtual void stop(){}

    const GPS* _gps;

    InputMethod type;

protected:
    int numFrames;
    int curFrameIndex ={0};
    bool isFinite;
    bool opened;
    bool calib = {false};
    bool bRecording = {false};

    int im_w = {0};
    int im_h = {0};

    const char* cfg_path="./videos/cfg";

    int digitSource;
    std::string strSource;

    cv::VideoWriter writer;
    CameraCalibration cam;
};

#endif // BASEIMAGEINPUT_H
