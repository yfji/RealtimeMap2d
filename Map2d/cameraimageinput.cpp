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

cv::Mat CameraImageInput::getRawImage(){
    cv::Mat image, correctedImage;
    if(opened){
        cap.read(image);
        if(calib){
            cv::resize(image, image, cv::Size(960,640), cv::INTER_LINEAR);
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
