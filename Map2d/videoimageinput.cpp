#include "videoimageinput.h"

VideoImageInput::VideoImageInput(const std::string& s):
    BaseImageInput(s)
{
    cap.open(strSource);
    opened=cap.isOpened();
    numFrames=cap.get(CV_CAP_PROP_FRAME_COUNT);
    isFinite=true;
}

cv::Mat VideoImageInput::getRawImage(){
    cv::Mat image;
    if(opened){
        cap.read(image);
        curFrameIndex++;
    }
    return image;
}
