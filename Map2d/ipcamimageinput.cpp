#include "ipcamimageinput.h"

IPCamImageInput::IPCamImageInput(const std::string& s):
    BaseImageInput(s)
{
    cap.open(strSource);
    opened=cap.isOpened();
    numFrames=-1;
    isFinite=false;
    calib=true;
    cam.initMatrix();

    _thread=std::thread(&IPCamImageInput::previewFunction,this);
    _thread.detach();
}

void IPCamImageInput::previewFunction(){
    std::cout<<"Start fetching IP stream"<<std::endl;
    while(opened && !_requestStop){
        cap.read(previewImage);
        usleep(100);
    }
    std::cout<<"Finish fetching stream"<<std::endl;
}

cv::Mat IPCamImageInput::getRawImage(){
    cv::Mat image, correctedImage;
    if(opened && !previewImage.empty()){
//        cap.read(image);
//        if(image.empty())
//            return image;

        if(calib){
#ifdef _1080P
            cv::resize(previewImage, previewImage, cv::Size(960,540), cv::INTER_LINEAR);
#else
            int start_y=(previewImage.rows-im_h)/2;
            int start_x=(previewImage.cols-im_w)/2;
            image=previewImage(cv::Rect(start_x, start_y, im_w, im_h));
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
