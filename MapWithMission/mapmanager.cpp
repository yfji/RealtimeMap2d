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
    else{
        use_image_thread=false;
        std::cout<<"Using stable image source"<<std::endl;
    }
    _mission.reset(new Mission());
    _input->_gps=&gps;
}

void MapManager::start(){
    if(use_image_thread){
        std::cout<<"Open image stream"<<std::endl;
        _image_thread=std::thread(&MapManager::getImageFunction, this);
        _image_thread.detach();
        usleep(1000);
    }

    _stitcher->reset();
    _thread=std::thread(&MapManager::threadFunction,this);
    _thread.detach();
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
    int numEmptyFrames=0;
    int checkFrames=5;
    int check_ind=0;
#ifdef USE_GPS
    vector<float> lon_lat;
#endif

    while(curState!=STOP){
        //img_mutex.lock();
        //curFrame=_input->getRawImage();
        //img_mutex.unlock();

        auto start=std::chrono::high_resolution_clock::now();
        if(!use_image_thread){
            img_mutex.lock();
            curFrame=_input->getRawImage();
#ifdef USE_GPS
            if(_input->type==VIDEO && !curFrame.empty())
                lon_lat=_input->getGPS();
#endif
            img_mutex.unlock();
            _mission_run=true;
        }
#ifdef USE_GPS
        if(_input->type==VIDEO)  {
            emit publishRecordGPS(lon_lat[0], lon_lat[1]);
        }
#endif
        if(curFrame.empty()){
            numEmptyFrames++;

            if(!use_image_thread){
                finished=true;
                curState=STOP;
                emit finishNatrual();
                break;
            }
            if(numEmptyFrames==50+checkFrames){
                finished=true;
                curState=STOP;
                break;
            }
            std::cout<<"Empty frame, wait"<<std::endl;
            usleep(100e3);
            continue;
        }
        else if(use_image_thread && check_ind<checkFrames){
            usleep(10e3);
            ++check_ind;
            std::cout<<"Check frame successfully"<<std::endl;
            continue;
        }
        cv::Mat stitchImage=equalize(curFrame);

        cv::Mat map2d;
        /*
        if(stitchImage.empty()){
            finished=true;
            curState=STOP;
            //callbackFunction(map2d, stitchImage);
            emit publishFrames(map2d, stitchImage);
            break;
        }
        //curFrame=equalize(curFrame);
        */
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
        //cv::imshow("map",map2d);
        //cv::waitKey(10);
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
    //_stitcher->reset();
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
        img_mutex.lock();
        if(curFrame.empty()){
            img_mutex.unlock();
            usleep(30e3);
            continue;
        }
        cv::Mat missionImage;
        cv::resize(curFrame, missionImage, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);
        //curFrame: 960x640
        //missionImage: 480x320
        //std::cout<<curFrame.rows<<','<<curFrame.cols<<std::endl;
        img_mutex.unlock();

        auto rects=_mission->findTargets(missionImage);
        auto& targets=_mission->targets;


        bool _gpsInHisory=false;
        /*
        if(targets.size()==0){
            _gpsInHisory=_mission->isGpsInHistory(gps);

            if(!_gpsInHisory)
                _mission->compareTargetsWithGps(rects, targets, gps, "iou", 0.1);
        }
        else{
            //_mission->compareTargets(rects, targets, "iou", 0.1);
            _mission->compareTargetsWithGps(rects, targets, gps, "iou", 0.1);
        }
        */
        if(!_gpsInHisory){
            _mission->compareTargetsWithGps(rects, targets, gps, "iou", 0.1);
        }

        /*
         * Save targets here per (check_interval) frames
         *
         */
        if(frame_index==check_interval){
            if(_mission->has_target){   //valid targets
                _gpsInHisory=_mission->isGpsInHistory(gps);
                if(!_gpsInHisory)
                    _mission->updateGPS(gps);
            }
            else
                _gpsInHisory=false;
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
    if(curState==BUILD || curState==STOP){
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
        _input->startRecord();
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
