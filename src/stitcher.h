#pragma once
#include "surf_feature.h"
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
	const int pix_thresh=90;//80
	const int MIN_MATCH_SIZE = {10};

	std::vector<int> multi_heights={240,320,360,480};

	float speed_x={0.};
	float speed_y={0.};

	float dist_max=0.0;
	const float dist_ratio_thresh = {0.3};

	char ignore = {0};

	std::shared_ptr<Feature> ptrFeature;

	cv::Point2i match_center;
	cv::Size stitch_size;
	cv::Rect box_area;
	cv::Point2f corners[4];
	cv::Point2i offset;
	cv::Point2i roi_offset;
	cv::Point2i box_offset;
	cv::Point2i last_offset;
	cv::Point2i center;
	cv::Point2i ori_center;
	cv::Point2i center_offset;

	cv::Point2i max_roi_offset;
	cv::Point2i min_roi_offset;
	cv::Mat stitchImage;
	cv::Mat map2d;
	cv::Mat lastFrame;
	cv::Mat lastTransFrame;
	cv::Mat lastWarpMatrix;

	cv::Mat getPatch();
	//cv::Mat getLastFrame();
	void calcWarpCorners(cv::Mat& warpMatrix);
	void optimize(cv::Mat& patch, cv::Mat& ref, cv::Mat& trans);
	void copyPatch(cv::Mat& patch, cv::Mat& trans, cv::Rect& pos);
	float getTransformRatio();
	void checkIfStitchable();
	void applyOffset();
	cv::Mat filter(cv::Mat image_ori);
	cv::Mat getWarpMatMultiscale(const cv::Mat& img_left, const cv::Mat& img_right);
	cv::Mat getWarpMat(const int h, const cv::Mat& img_left, const cv::Mat& img_right);
};

