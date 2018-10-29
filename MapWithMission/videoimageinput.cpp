#include "videoimageinput.h"

VideoImageInput::VideoImageInput(const std::string& s):
    BaseImageInput(s)
{
    type=VIDEO;
    cap.open(strSource);
    opened=cap.isOpened();
    numFrames=cap.get(CV_CAP_PROP_FRAME_COUNT);
    isFinite=true;
    calib=true;
    cam.initMatrix();
    im_w=960;
    im_h=540;

#ifdef USE_GPS
    int index=getVideoIndex(s);
    std::stringstream ss;
    ss<<"logs/gps_"<<index<<".log";
    log_path=ss.str();

    if(log_in){
        log_in.close();
    }
    log_in.open(log_path.c_str(), std::ios::in);
    if(log_in){
        std::cout<<"Open "<<log_path<<" successfully"<<std::endl;
    }
    else{
        std::cout<<"Open "<<log_path<<" failed"<<std::endl;
    }
#endif
}

std::vector<float> VideoImageInput::getGPS(){
    float lon, lat;
    log_in>>lon>>lat;
    return {lon, lat};
}

int VideoImageInput::getVideoIndex(const string &filename){
    int len=filename.length();
    char index_data[10];
    int i=len-1;
    for(;i>=0;--i){
        if(filename[i]=='_')
            break;
    }
    if(i<0){
        return -1;
    }
    ++i;
    int start=i;
    for(;i<len;++i){
        index_data[i-start]=filename[i];
    }
    index_data[i]='\0';
    std::stringstream ss;
    ss<<std::string(index_data);
    int index=0;
    ss>>index;
    return index;
}

cv::Mat VideoImageInput::getRawImage(){
    cv::Mat image, correctedImage;
    if(opened){
        cap.read(image);
        if(image.empty())
            return image;
        if(calib){
#ifdef _1080P
            cv::resize(image, image, cv::Size(960,540), cv::INTER_LINEAR);
#else
            int start_y=(image.rows-im_h)/2;
            int start_x=(image.cols-im_w)/2;
            image=image(cv::Rect(start_x, start_y, im_w, im_h));
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
