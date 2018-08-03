#pragma once
//#include "surf_feature.h"
#include "orb_feature.h"
#include <memory>

class ImageStitcher
{
public:
	ImageStitcher(int w, int h);
	~ImageStitcher();

public:
	void stitch(cv::Mat& img);
	inline cv::Mat getStitchedImage() {
		return map2d;
	}
private:
	char firstImg = { 1 };
	int width;
	int height;
	const int pad_x={400};
	const int pad_y={400};

	std::shared_ptr<Feature> ptrFeature;

	cv::Point2i match_center;
	cv::Size stitch_size;
	cv::Rect box_area;
	cv::Point2f corners[4];
	cv::Point2i offset;
	cv::Point2i map_offset;
	cv::Mat stitchImage;
	cv::Mat map2d;

	cv::Mat getPatch();
	void calcWarpCorners(cv::Mat& warpMatrix);
	void optimize(cv::Mat& patch, cv::Mat& ref, cv::Mat& trans);
	void copyPatch(cv::Mat& patch, cv::Mat& trans, cv::Rect& pos);
	void applyOffset();
};

