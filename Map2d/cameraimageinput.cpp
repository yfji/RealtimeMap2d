#include "cameraimageinput.h"

CameraImageInput::CameraImageInput(int d):
    BaseImageInput(d)
{
    cap.open(digitSource);
    opened=cap.isOpened();
    numFrames=-1;
    isFinite=false;
}

cv::Mat CameraImageInput::getRawImage(){
    cv::Mat image;
    if(opened){
        cap.read(image);
        curFrameIndex++;
    }
    return image;
}
