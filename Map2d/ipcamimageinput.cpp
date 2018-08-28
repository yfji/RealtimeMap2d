#include "ipcamimageinput.h"

IPCamImageInput::IPCamImageInput(const std::string& s):
    BaseImageInput(s)
{
    cap.open(strSource);
    opened=cap.isOpened();
    numFrames=-1;
    isFinite=false;
}

cv::Mat IPCamImageInput::getRawImage(){
    cv::Mat image;
    if(opened){
        cap.read(image);
        curFrameIndex++;
    }
    return image;
}
