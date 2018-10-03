#include "feature.h"
#include <opencv2/xfeatures2d/nonfree.hpp>
#include "gms_matcher.h"

class OrbFeature :
	public Feature
{
public:
	OrbFeature();
	virtual ~OrbFeature();

	virtual void getMatches(std::vector<cv::Point2f>& pt_left, std::vector<cv::Point2f>& pt_right);

private:
	cv::Ptr<cv::ORB> orb;
};