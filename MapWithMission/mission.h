#ifndef MISSION_H
#define MISSION_H

#include "salient.h"
#include <map>
#include <tuple>
#include <vector>


enum TYPE {
    BARREL,
    CROCODILE,
    SURVIVOR
};

struct GPS{
    float lan;
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

private:
    std::vector<Target> targets;
    GPS currentGPS;

    const int barrel_thresh = {80};
    const int barrel_diff_thresh ={40};
    const int croco_thresh = {120};
    const int croco_diff_thresh = {20};

    std::vector<std::pair<cv::Rect, TYPE> > findBoundingBoxes(cv::Mat& rgbImg, cv::Mat& biImg, \
                                            int min_area=200, \
                                            int max_area=40000);

    bool isBarrel(std::tuple<int,int,int>& count, cv::Rect& rect, float min_ratio=0.2, float max_ratio=4);//ratio:w/h

    bool isCrocodile(std::tuple<int,int,int>& count, cv::Rect& rect, float min_ratio=0.8, float max_ratio=1.5);

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
};

#endif // MISSION_H
