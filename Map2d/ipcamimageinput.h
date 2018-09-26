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

private:
    cv::VideoCapture cap;

    const int im_h=540;
    const int im_w=960;

    cv::Mat previewImage;
    std::thread _thread;

    void previewFunction();
};

#endif // IPCAMIMAGEINPUT_H
