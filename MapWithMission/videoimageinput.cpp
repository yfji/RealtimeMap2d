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
        if(image.empty())
            return image;
        if(calib){
#ifdef _1080P
            cv::resize(image, image, cv::Size(960,540), cv::INTER_LINEAR);
#else
            int start_y=(image.rows-im_h)/2;
            int start_x=(image.cols-im_w)/2;
            image=image(cv::Rect(start_x, start_y, im_w, im_h));
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
