#ifndef IPCAMIMAGEINPUT_H
#define IPCAMIMAGEINPUT_H
#include "baseimageinput.h"
#include <thread>
#include <mutex>
#include <unistd.h>

class IPCamImageInput : public BaseImageInput
{
public:
    IPCamImageInput(const std::string& s);

    virtual cv::Mat getRawImage();

    virtual void release(){
        if(opened){
            cap.release();
            opened=false;
        }
    }
    virtual void stop(){
        _requestStop=true;
        usleep(10000);
    }

private:
    cv::VideoCapture cap;

    std::mutex _mutex;

    const int im_h=540;
    const int im_w=960;

    bool _requestStop= {false};
    cv::Mat previewImage;
    std::thread _thread;

    void previewFunction();
};

#endif // IPCAMIMAGEINPUT_H