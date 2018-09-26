#ifndef IPCAMIMAGEINPUT_H
#define IPCAMIMAGEINPUT_H
#include "baseimageinput.h"
#include <thread>
#include <unistd.h>

class IPCamImageInput : public BaseImageInput
{
public:
    IPCamImageInput(const std::string& s);

    virtual cv::Mat getRawImage();

    virtual void release(){
        if(opened)
            cap.release();
    }
    virtual void stop(){
        _requestStop=true;
    }

private:
    cv::VideoCapture cap;

    const int im_h=540;
    const int im_w=960;

    bool _requestStop= {false};
    cv::Mat previewImage;
    std::thread _thread;

    void previewFunction();
};

#endif // IPCAMIMAGEINPUT_H
