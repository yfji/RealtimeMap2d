#include "feature.h"
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
	const int minHessian = { 100 };
	const float thresh_ratio = { 1.5f };
	
	cv::Ptr<cv::xfeatures2d::SURF> ptrDetector;
	cv::Ptr<cv::xfeatures2d::SURF> ptrExtractor;
};

