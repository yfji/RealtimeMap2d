#include "cameraimageinput.h"

CameraImageInput::CameraImageInput(int d):
    BaseImageInput(d)
{
    cap.open(digitSource);
    opened=cap.isOpened();
    numFrames=-1;
    isFinite=false;
    calib=true;
    cam.initMatrix();
}

CameraImageInput::CameraImageInput(const std::string& s):
    BaseImageInput(s)
{
    cap.open(s);
    opened=cap.isOpened();
    numFrames=-1;
    isFinite=false;
    //calib=true;
    //cam.initMatrix();
}

cv::Mat CameraImageInput::getRawImage(){
    cv::Mat image, correctedImage;
    if(opened){
        cap.read(image);
        if(image.empty())
            return image;
        if(calib){
#ifdef _1080P
            cv::resize(image, image, cv::Size(960,540), cv::INTER_LINEAR);
#else
            cv::resize(image, image, cv::Size(960,540), cv::INTER_LINEAR);
#endif
            if (correctedImage.empty())
                correctedImage = cv::Mat::zeros(image.size(), image.type());
            cam.getInterpImage(image, correctedImage);
        }
        else{
            correctedImage=image;
        }
        curFrameIndex++;
    }
    return correctedImage;
}
