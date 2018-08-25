#include "mapmanager.h"

MapManager::MapManager()
{
    _stitcher.reset(new ImageStitcher(frame_w, frame_h));
}

void MapManager::open(InputMethod method){
    this->method=method;
    opened=true;
    finished=false;
    if(method==CAMERA){
        _input.reset(new CameraImageInput(cam_id));
    }
    else if(method==VIDEO){
        _input.reset(new VideoImageInput(videoFile));
    }
    else if(method==FILELIST){
        _input.reset(new FileListImageInput(fileList));
    }
}

void MapManager::start(){
    _stitcher->reset();
    _thread=std::thread(&MapManager::threadFunction,this);
    _thread.detach();
}

void MapManager::threadFunction(){
    char time_reached=true;
    float duration=0;
    while(curState!=STOP){
        cv::Mat map2d;
        auto start=std::chrono::high_resolution_clock::now();
        curFrame=_input->getRawImage();
        cv::Mat frame;        
        if(curFrame.empty()){
            finished=true;
            break;
        }
        //int start_x=(curFrame.cols-frame_w)/2;
        //int start_y=(curFrame.rows-frame_h)/2;
        //frame=curFrame(cv::Rect(start_x, start_y, frame_w, frame_h));

        cv::resize(curFrame, frame, cv::Size(frame_w, frame_h), cv::INTER_LINEAR);
        if(curState==PAUSE){
            if(time_reached){
                _stitcher->matchNoStitch(frame);
            }
            map2d= _stitcher->getMatchedImage();
        }
        else if(curState==BUILD){
            if(time_reached){
                _stitcher->stitch(frame);
            }
            map2d = _stitcher->getStitchedImage();
        }
        if(!time_reached){
            usleep(50e3);
        }
        time_reached=false;
        std::cout<<"Stitching frame "<<curIndex<<std::endl;
        curIndex++;
        callbackFunction(map2d, curFrame);

        auto now=std::chrono::high_resolution_clock::now();
        float duration_us=(float)std::chrono::duration_cast<std::chrono::microseconds>(now-start).count();
        float micro_seconds=period*1e6;
        duration+=duration_us;

        if(duration>=micro_seconds){
            //usleep(micro_seconds-duration_us);
            time_reached=true;
            duration=0;
        }
    }
    _input->release();
    opened=false;
}

cv::Mat MapManager::getImage(){
    cv::Mat image;
    if(curState==BUILD){
        image=_stitcher->getStitchedImage();
    }
    else if(curState==PAUSE){
        image=_stitcher->getMatchedImage();
    }
    return image;
}
