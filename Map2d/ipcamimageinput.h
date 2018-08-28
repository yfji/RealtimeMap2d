#ifndef IPCAMIMAGEINPUT_H
#define IPCAMIMAGEINPUT_H
#include "baseimageinput.h"

class IPCamImageInput : public BaseImageInput
{
public:
    IPCamImageInput(const std::string& s);

    virtual cv::Mat getRawImage();

private:
    cv::VideoCapture cap;
};

#endif // IPCAMIMAGEINPUT_H
