#ifndef CAMERAIMAGEINPUT_H
#define CAMERAIMAGEINPUT_H
#include "baseimageinput.h"

class CameraImageInput : public BaseImageInput
{
public:
    CameraImageInput(int d);
    CameraImageInput(const std::string& s);

    virtual cv::Mat getRawImage();

    virtual void release(){
        if(opened){
            cap.release();
        }
    }

private:
    cv::VideoCapture cap;
};

#endif // CAMERAIMAGEINPUT_H
