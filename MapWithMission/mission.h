#ifndef MISSION_H
#define MISSION_H

#include "salient.h"
#include <map>
#include <tuple>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>

#define RIGHT_ANGLE 90.0
#define EARTH_RADIUS 6378.137

enum TYPE {
    BARREL,
    CROCODILE,
    SURVIVOR
};

struct GPS{
    float lon;  //jingdu
    float lat;  //weidu
};

struct Target{
    GPS gpsLocation;
    cv::Rect location;
    int id;
    int life;
    int forgot;
    TYPE type;
};

static std::string lonlat2degreefenmiao(double lonlat)
{
    double degree = double(int(lonlat));
    double fen = double(lonlat - degree) * 60;
    double miao = (fen - (int)fen) * 60;
    fen = int(fen);

    if (fabs(miao) < 0.00000001)
    {
        miao = 0.0;
    }

    std::stringstream ss;
    ss << degree << "°" << fen << "'" << miao << "''";
    std::string result;
    ss >> result;

    return result;
}

/**
 * @brief get rad according to the degrees.
 * @param degree:
 *
 * @return: rad according to the degrees.
 */
static double Deg2Rad(double degree)
{
    return degree * CV_PI / (RIGHT_ANGLE * 2);
}

/**
 * @brief calc the direction distance of two coordinates.
 * @param srcLon: the longitude of source coordinate;
 *        srcLat: the latitude of source coordinate;
 *        destLon: the longitude of destination coordinate;
 *        destLat: the latitude of destination coordinate;
 *
 * @return DirectDistance: the direction distance from source coordinate to destination coordinate.
 */
static double GetDirectDistance(double srcLat, double srcLon, double destLat, double destLon)
{
    double radSrcLat = Deg2Rad(srcLat);
    double radDestLat = Deg2Rad(destLat);
    double a = radSrcLat - radDestLat;
    double b = Deg2Rad(srcLon) - Deg2Rad(destLon);

    double DirectDistance = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(radSrcLat) * cos(radDestLat) * pow(sin(b / 2), 2)));

    DirectDistance = DirectDistance * EARTH_RADIUS;
    DirectDistance = round(DirectDistance * 10000) / 10000 * 1000;

    return DirectDistance;
}


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

    bool has_target = {false};

private:
    int savedIndex = {0};
    std::ofstream out_det;

    bool gpsInHistory = {false};

    GPS currentGPS;

    cv::Mat currentImage;

    const int barrel_thresh = {120};
    const int barrel_diff_thresh ={30};
    const int croco_thresh = {120};
    const int croco_diff_thresh = {30};

    const float gps_dist_thresh={20};

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

    std::vector<std::pair<cv::Rect, TYPE> > findTargets(cv::Mat& oriImg, const int num_threads=12);

    void saveTargets();
    void saveGPS();

    bool isGpsInHistory(GPS& curGps);

};

#endif // MISSION_H
