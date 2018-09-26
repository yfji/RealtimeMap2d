#ifndef VIDEOIMAGEINPUT_H
#define VIDEOIMAGEINPUT_H

#include "baseimageinput.h"
class VideoImageInput: public BaseImageInput
{
public:
    VideoImageInput(const std::string& s);

    virtual cv::Mat getRawImage();
    virtual void release(){
        if(opened)
            cap.release();
    }

private:
    cv::VideoCapture cap;

    const int im_h=720;
    const int im_w=1280;
};

#endif // VIDEOIMAGEINPUT_H
