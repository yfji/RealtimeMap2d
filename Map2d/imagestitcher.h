#ifndef IMAGESTITCHER_H
#define IMAGESTITCHER_H

#pragma once
#include "orb_feature.h"
#include <memory>

class ImageStitcher
{
public:
    ImageStitcher(int w, int h);
    ~ImageStitcher();

public:
    void stitch(cv::Mat& img);
    void matchNoStitch(cv::Mat& img);

    inline cv::Mat getStitchedImage(bool withRect=true) {
        if(withRect)
            return map2dCanvas;
        return map2d;
    }
    inline cv::Mat getMatchedImage(bool withRect= true){
        if(withRect)
            return map2dCanvas;
        return map2dNoStitch;
    }
    inline std::string getState(){
        return state;
    }
    inline void updateAlphaOffset(float _a){
        _alpha_offset=_a;
    }
    inline void updateAlphaOptim(float _a){
        _alpha_optim=_a;
    }
    bool showORB= {false};
    void reset();

private:
    char firstImg = { 1 };
    int width;
    int height;
    const int pad_x={400};
    const int pad_y={400};
    const int pix_thresh=50;//80
    const int MIN_MATCH_SIZE = {10};

    float speed_x={0.};
    float speed_y={0.};

    float _alpha_offset = {0.3};
    float _alpha_optim = {0.6};

    float dist_max=0.0;

    char ignore = {0};
    std::string state;

    std::shared_ptr<Feature> ptrFeature;

    cv::Point2i match_center;
    cv::Size stitch_size;
    cv::Rect box_area;
    cv::Point2f corners[4];
    cv::Point2i offset;

    cv::Point2i max_roi_offset;
    cv::Point2i min_roi_offset;
    cv::Mat stitchImage;
    cv::Mat map2d;
    cv::Mat map2dNoStitch;
    cv::Mat map2dCanvas;

    cv::Mat getPatch();
    void calcWarpCorners(cv::Mat& warpMatrix);
    void optimize(cv::Mat& patch, cv::Mat& ref, cv::Mat& trans);
    void copyPatch(cv::Mat& patch, cv::Mat& trans, cv::Rect& pos);
    void checkIfStitchable();
    void applyOffset();
};

#endif // IMAGESTITCHER_H
