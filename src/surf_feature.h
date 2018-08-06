#include "feature.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <memory>

class SurfFeature :
	public Feature
{
public:
	SurfFeature();
	virtual ~SurfFeature();

	virtual void getMatches(std::vector<cv::Point2f>& pt_left, std::vector<cv::Point2f>& pt_right);

private:
	const int minHessian = { 10000 };
	const float thresh_ratio = { 1.5f };
	
	cv::Ptr<cv::xfeatures2d::SURF> ptrDetector;
	
	inline void imresize(cv::Mat &src, int height) {
		double ratio = src.rows * 1.0 / height;
		int width = static_cast<int>(src.cols * 1.0 / ratio);
		resize(src, src, cv::Size(width, height));
	}

};

