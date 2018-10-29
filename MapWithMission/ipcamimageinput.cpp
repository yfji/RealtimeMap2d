#include "ipcamimageinput.h"

IPCamImageInput::IPCamImageInput(const std::string& s):
    BaseImageInput(s)
{
    type=IPCAMERA;
    cap.open(strSource);
    opened=cap.isOpened();
    numFrames=-1;
    isFinite=false;
    calib=true;
    cam.initMatrix();
    im_w=960;
    im_h=540;

#ifdef SAVE_GPS
    std::ifstream in;
    in.open(cfg_path, std::ios::in);
    int index;
    in>>index;
    in.close();

    std::stringstream ss;
    ss<<"logs/gps_"<<index-1<<".log";

    out_gps.open(ss.str().c_str(), std::ios::out);

#endif

    _thread=std::thread(&IPCamImageInput::readStreamFunction,this);
    _thread.detach();
}

void IPCamImageInput::readStreamFunction(){
    std::cout<<"Start fetching IP stream"<<std::endl;
    while(opened && !_requestStop){
        _mutex.lock();
        cap.read(previewImage);
        _mutex.unlock();
        usleep(10);
    }
    if(!opened){
        std::cout<<"Open ip stream failed"<<std::endl;
    }
    if(_requestStop){
        std::cout<<"Request stop triggered"<<std::endl;
    }
    std::cout<<"Finish fetching stream"<<std::endl;
}

cv::Mat IPCamImageInput::getRawImage(){
    cv::Mat image, correctedImage;
    if(opened && !_requestStop){
//        cap.read(image);
//        if(image.empty())
//            return image;
        _mutex.lock();
        cv::Mat tempImg=previewImage.clone();
        _mutex.unlock();

        if(tempImg.empty()){
            return cv::Mat();
        }

        if(calib){
#ifdef _1080P
            cv::resize(tempImg, tempImg, cv::Size(960,540), cv::INTER_LINEAR);
#else
            int start_y=(tempImg.rows-im_h)/2;
            int start_x=(tempImg.cols-im_w)/2;
            image=tempImg(cv::Rect(start_x, start_y, im_w, im_h));
            if(bRecording)
                writer<<image;
#ifdef SAVE_GPS
            char _lon[20];
            char _lat[20];
            sprintf(_lon, "%.15f",_gps->lon);
            sprintf(_lat, "%.15f",_gps->lat);

            out_gps<<_lon<<' '<<_lat<<std::endl;
#endif

#endif
            if (correctedImage.empty())
                correctedImage = cv::Mat::zeros(image.size(), image.type());
            cam.getInterpImage(image, correctedImage);
            cv::resize(correctedImage, correctedImage, cv::Size(960, 640), cv::INTER_LINEAR);
        }
        else{
            correctedImage=image;
        }
        curFrameIndex++;
    }
    return correctedImage;
}
