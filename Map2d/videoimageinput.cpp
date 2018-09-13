#include "videoimageinput.h"

VideoImageInput::VideoImageInput(const std::string& s):
    BaseImageInput(s)
{
    cap.open(strSource);
    opened=cap.isOpened();
    numFrames=cap.get(CV_CAP_PROP_FRAME_COUNT);
    isFinite=true;
    calib=true;
    cam.initMatrix();
}

cv::Mat VideoImageInput::getRawImage(){
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
