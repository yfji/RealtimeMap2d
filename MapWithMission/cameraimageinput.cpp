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
    im_w=960;
    im_h=540;
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
    im_w=960;
    im_h=540;
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
            cv::resize(image, image, cv::Size(im_w, im_h), cv::INTER_LINEAR);
            if(bRecording)
                writer<<image;
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
