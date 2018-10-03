#ifndef VIDEOIMAGEINPUT_H
#define VIDEOIMAGEINPUT_H

#include "baseimageinput.h"
class VideoImageInput: public BaseImageInput
{
public:
    VideoImageInput(const std::string& s);

    virtual cv::Mat getRawImage();
    virtual void release(){
        if(opened){
            cap.release();
            opened=false;
        }
    }

private:
    cv::VideoCapture cap;

    const int im_h=540;
    const int im_w=960;
};

#endif // VIDEOIMAGEINPUT_H
