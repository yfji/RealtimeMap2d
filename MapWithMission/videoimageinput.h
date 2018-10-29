#ifndef VIDEOIMAGEINPUT_H
#define VIDEOIMAGEINPUT_H

#include "baseimageinput.h"

#define USE_GPS

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
    std::vector<float> getGPS();

private:
    cv::VideoCapture cap;

    //const int im_h=540;
    //const int im_w=960;
    std::string log_path;
    std::ifstream log_in;
    int getVideoIndex(const std::string& filename);
};

#endif // VIDEOIMAGEINPUT_H
