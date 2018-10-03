#ifndef MISSION_H
#define MISSION_H

#include "salient.h"
#include <map>
#include <tuple>
#include <vector>
#include <fstream>
#include <sstream>


enum TYPE {
    BARREL,
    CROCODILE,
    SURVIVOR
};

struct GPS{
    float lat;
    float lon;
};

struct Target{
    GPS gpsLocation;
    cv::Rect location;
    int id;
    int life;
    int forgot;
    TYPE type;
};

class Mission
{
public:
    Mission();
    ~Mission();

    std::vector<Target>  targets;
    std::vector<GPS> historyGps;

    inline GPS getGPS(){
        return currentGPS;
    }
    inline void setGPS(const GPS& g){
        currentGPS=g;
    }

private:
    int savedIndex = {0};
    std::ofstream out;

    GPS currentGPS;

    cv::Mat currentImage;

    const int barrel_thresh = {80};
    const int barrel_diff_thresh ={40};
    const int croco_thresh = {120};
    const int croco_diff_thresh = {20};

    const float gps_dist_thresh={100};

    std::vector<std::pair<cv::Rect, TYPE> > findBoundingBoxes(cv::Mat& rgbImg, cv::Mat& biImg, \
                                            int min_area=200, \
                                            int max_area=40000);

    bool isBarrel(std::tuple<int,int,int>& count, cv::Rect& rect, float min_ratio=0.2, float max_ratio=4);//ratio:w/h

    bool isCrocodile(std::tuple<int,int,int>& count, cv::Rect& rect, float min_ratio=0.8, float max_ratio=1.5);

    void updateGPS(const GPS& g);

public:
    void compareTargets(std::vector<std::pair<cv::Rect, TYPE> >& locations, \
                        std::vector<Target>& targets, \
                        const char* mode="iou", \
                        const float overlap=0.7,\
                        const float eps=10, \
                        const int forgetFrame=3);

    void compareTargetsWithGps(std::vector<std::pair<cv::Rect, TYPE> >& locations,
                        std::vector<Target>& targets, \
                        GPS currentGps, \
                        const char* mode="iou", \
                        const float overlap=0.7,\
                        const float eps=10, \
                        const int forgetFrame=3);

    std::vector<std::pair<cv::Rect, TYPE> > findTargets(cv::Mat& oriImg);

    void saveTargets();

};

#endif // MISSION_H
