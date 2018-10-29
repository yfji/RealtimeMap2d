#include "baseimageinput.h"

BaseImageInput::BaseImageInput(int d)
{
    digitSource=d;
}

BaseImageInput::BaseImageInput(const std::string &s){
    strSource=s;
}

void BaseImageInput::startRecord(){
    std::ifstream in;
    in.open(cfg_path, std::ios::in);
    int index;
    in>>index;
    in.close();

    std::stringstream ss;
    ss<<"videos/video_"<<index<<".avi";

    writer.open(ss.str(), CV_FOURCC('M','J','P','G'), 30, cv::Size(im_w, im_h));
    bRecording=(writer.isOpened());

    index++;
    std::ofstream out;
    out.open(cfg_path, std::ios::out);
    out<<index;
    out.close();
}
