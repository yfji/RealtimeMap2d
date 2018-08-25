#ifndef VIDEOIMAGEINPUT_H
#define VIDEOIMAGEINPUT_H

#include "baseimageinput.h"
class VideoImageInput: public BaseImageInput
{
public:
    VideoImageInput(const std::string& s);

    virtual cv::Mat getRawImage();

private:
    cv::VideoCapture cap;
};

#endif // VIDEOIMAGEINPUT_H