#include "mapmanager.h"

MapManager::MapManager(QObject* parent):
    QObject(parent)
{
    _stitcher.reset(new ImageStitcher(frame_w, frame_h));
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<cv::Mat>("cv::Mat&");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<std::string>("std::string&");

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
    if(method==IPCAMERA || method==CAMERA){
        use_image_thread=true;
        std::cout<<"Using image thread"<<std::endl;
    }
    _mission.reset(new Mission());
}

void MapManager::start(){
    _stitcher->reset();
    _thread=std::thread(&MapManager::threadFunction,this);
    _thread.detach();

    if(use_image_thread){
        _image_thread=std::thread(&MapManager::getImageFunction, this);
        _image_thread.detach();

    }
}

void MapManager::getImageFunction(){
    while(curState!=STOP){
        img_mutex.lock();
        curFrame=_input->getRawImage();
        img_mutex.unlock();
        usleep(1000);
    }
}

void MapManager::threadFunction(){
    char time_reached=true;
    float duration=0;
    while(curState!=STOP){
        auto start=std::chrono::high_resolution_clock::now();
        if(!use_image_thread){
            img_mutex.lock();
            curFrame=_input->getRawImage();
            img_mutex.unlock();
        }

        if(curFrame.empty()){
            //usleep(10e3);
            //continue;
            finished=true;
            curState=STOP;
            break;
        }
        cv::Mat stitchImage=equalize(curFrame);
        _mission_run=true;

        cv::Mat map2d;
        if(stitchImage.empty()){
            finished=true;
            curState=STOP;
            //callbackFunction(map2d, stitchImage);
            emit publishFrames(map2d, stitchImage);
            break;
        }
        //curFrame=equalize(curFrame);

        if(curState==PREVIEW){
            //callbackFunction(map2d, curFrame);
            emit publishFrames(map2d, stitchImage);
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
        cv::Mat frame = stitchImage;
        if(stitchImage.cols!=frame_w || stitchImage.rows!=frame_h)
            cv::resize(frame, frame, cv::Size(frame_w, frame_h), cv::INTER_LINEAR);

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
        /*
         *
         * Determines the frequency to read frames
         * Usually, the minimum stitching time is 0.2s
         * If set the time interval less than 0.2s on the panel
         * The fps gets slower as the reading and stitching is in the same thread
         * If set the time interval more than 0.2s
         * The fps is 1/10e3 according to the usleep below during (interval-stitching_time)
         *
        */
        if(!time_reached){
            usleep(10e3);
        }
        time_reached=false;
        //callbackFunction(map2d, curFrame);
        std::string state=_stitcher->getState();
        emit publishStates(state);
        emit publishFrames(map2d, stitchImage);

        auto now=std::chrono::high_resolution_clock::now();
        float duration_us=(float)std::chrono::duration_cast<std::chrono::microseconds>(now-start).count();
        micro_seconds=period*1e6;
        duration+=duration_us;

        if(duration>=micro_seconds){
            //usleep(micro_seconds-duration_us);
            time_reached=true;
            duration=0;
        }
        //std::cout<<"Stitching frame "<<curIndex<<std::endl;
        curIndex++;
    }
    _input->stop();
    _input->release();
    opened=false;

}

void MapManager::missionFunction(){
    while(curState!=STOP && detecting){
        if(!use_image_thread && !_mission_run){
            usleep(5e3);
            continue;
        }
        if(curFrame.empty()){
            usleep(5e3);
            continue;
        }
        cv::Mat missionImage;
        img_mutex.lock();
        cv::resize(curFrame, missionImage, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);
        img_mutex.unlock();
        auto rects=_mission->findTargets(missionImage);
        auto& targets=_mission->targets;


        if(targets.size()==0){
            _mission->isGpsInHistory(gps);
                _mission->compareTargetsWithGps(rects, targets, gps, "iou", 0.1);
        }
        else{
            //_mission->compareTargets(rects, targets, "iou", 0.1);
            _mission->compareTargetsWithGps(rects, targets, gps, "iou", 0.1);
        }


        /*
         * Save targets here per (check_interval) frames
         *
         */
        if(frame_index==check_interval){
            _mission->saveTargets();
            frame_index=0;
        }
        ++frame_index;
        _mission_run=false;
    }
    cv::destroyAllWindows();
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

void MapManager::record(bool flag){
    if(flag){
        std::stringstream ss;
        ss<<"videos/video_"<<videoIndex<<".avi";
        _input->startRecord(ss.str());
    }
    else
        _input->endRecord();
}

void MapManager::detect(bool flag){
    if(flag){
        detecting=true;
        _mission_thread=std::thread(&MapManager::missionFunction, this);
        _mission_thread.detach();
    }
    else{
        detecting=false;
    }
}
