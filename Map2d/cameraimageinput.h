#ifndef CAMERAIMAGEINPUT_H
#define CAMERAIMAGEINPUT_H
#include "baseimageinput.h"

class CameraImageInput : public BaseImageInput
{
public:
    CameraImageInput(int d);

    virtual cv::Mat getRawImage();

    virtual void release(){
        if(cap.isOpened()){
            cap.release();
        }
    }

private:
    cv::VideoCapture cap;
};

#endif // CAMERAIMAGEINPUT_H
