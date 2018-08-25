#ifndef BASEIMAGEINPUT_H
#define BASEIMAGEINPUT_H

#include <opencv2/opencv.hpp>
#include <iostream>

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

    virtual cv::Mat getRawImage()=0;

    virtual void release(){}

protected:
    int numFrames;
    int curFrameIndex ={0};
    bool isFinite;
    bool opened;

    int digitSource;
    std::string strSource;
};

#endif // BASEIMAGEINPUT_H
