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

    inline cv::Mat getStitchedImage() {
        return map2d;
    }
    inline cv::Mat getMatchedImage(){
        return map2dCanvas;
    }

    void reset();

private:
    char firstImg = { 1 };
    int width;
    int height;
    const int pad_x={400};
    const int pad_y={400};
    const int pix_thresh=90;//80
    const int MIN_MATCH_SIZE = {10};

    float speed_x={0.};
    float speed_y={0.};

    float dist_max=0.0;

    char ignore = {0};

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
