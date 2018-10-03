#include "mapmanager.h"

MapManager::MapManager(QObject* parent):
    QObject(parent)
{
    _stitcher.reset(new ImageStitcher(frame_w, frame_h));
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<cv::Mat>("cv::Mat&");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<std::string>("std::string&");

    _mission.reset(new Mission());
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
    else if(method==IPCAMERA){
        _input.reset(new IPCamImageInput(camIP));
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
        auto start=std::chrono::high_resolution_clock::now();
        curFrame=_input->getRawImage();

        curFrame=equalize(curFrame);
        _mission_run=true;

        cv::Mat map2d;
        if(curFrame.empty()){
            finished=true;
            curState=STOP;
            //callbackFunction(map2d, curFrame);
            emit publishFrames(map2d, curFrame);
            break;
        }
        //curFrame=equalize(curFrame);

        if(curState==PREVIEW){
            //callbackFunction(map2d, curFrame);
            emit publishFrames(map2d, curFrame);
            auto now=std::chrono::high_resolution_clock::now();
            float duration_us=(float)std::chrono::duration_cast<std::chrono::microseconds>(now-start).count();
            micro_seconds=period*1e6;
            if(duration_us<micro_seconds){
                usleep(micro_seconds-duration_us);
            }
            continue;
        }
        //int start_x=(curFrame.cols-frame_w)/2;
        //int start_y=(curFrame.rows-frame_h)/2;
        //frame=curFrame(cv::Rect(start_x, start_y, frame_w, frame_h));
        cv::Mat frame;
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
            usleep(20e3);
        }
        time_reached=false;
        //callbackFunction(map2d, curFrame);
        std::string state=_stitcher->getState();
        emit publishStates(state);
        emit publishFrames(map2d, curFrame);

        auto now=std::chrono::high_resolution_clock::now();
        float duration_us=(float)std::chrono::duration_cast<std::chrono::microseconds>(now-start).count();
        micro_seconds=period*1e6;
        duration+=duration_us;

        if(duration>=micro_seconds){
            //usleep(micro_seconds-duration_us);
            time_reached=true;
            duration=0;
        }
        std::cout<<"Stitching frame "<<curIndex<<std::endl;
        curIndex++;
    }
    _input->stop();
    _input->release();
    opened=false;
    //cv::destroyAllWindows();
}

void MapManager::missionFunction(){
    while(curState!=STOP){
        if(!_mission_run){
            usleep(30e3);
            continue;
        }

        cv::Mat missionImage;
        cv::resize(curFrame, missionImage, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);
        auto rects=_mission->findTargets(missionImage);
        auto& targets=_mission->targets;
        if(targets.size()==0){
            _mission->compareTargetsWithGps(rects, targets, gps, "iou", 0.5);
        }
        else{
            _mission->compareTargets(rects, targets, "iou", 0.5);
        }
        _mission_run=false;
    }
}

cv::Mat MapManager::getImage(bool withRect){
    cv::Mat image;
    if(curState==BUILD){
        image=_stitcher->getStitchedImage(withRect);
    }
    else if(curState==PAUSE){
        image=_stitcher->getMatchedImage(withRect);
    }
    return image;
}

cv::Mat MapManager::equalize(cv::Mat& image){
    cv::Mat out;
    cv::normalize(image, out, 0, 255, cv::NORM_MINMAX);
    return out;
}
