#include "surf_feature.h"


SurfFeature::SurfFeature()
{
	ptrDetector= cv::xfeatures2d::SURF::create(minHessian);
	ptrExtractor = cv::xfeatures2d::SURF::create();
}


SurfFeature::~SurfFeature()
{
}

void SurfFeature::getMatches(std::vector<cv::Point2f>& pt_left, std::vector<cv::Point2f>& pt_right) {
	std::vector<cv::KeyPoint> keyPoint1, keyPoint2;  
	
	cv::Mat imageDesc1, imageDesc2;
	ptrDetector->detect(image1, keyPoint1, cv::Mat());
	ptrDetector->detect(image2, keyPoint2, cv::Mat());
	//ptrDetector->detectAndCompute(image1, cv::Mat(), keyPoint1, imageDesc1);	//fail!
	//ptrDetector->detectAndCompute(image2, cv::Mat(), keyPoint2, imageDesc1);
	ptrExtractor->compute(image1, keyPoint1, imageDesc1);
	ptrExtractor->compute(image2, keyPoint2, imageDesc2);

	std::vector<cv::DMatch> matches;
	std::vector<cv::DMatch> GoodMatches;

	std::vector<cv::Mat> train_desc(1, imageDesc1);

	cv::Ptr<cv::DescriptorMatcher> ptrMatcher = cv::DescriptorMatcher::create("FlannBased");
	ptrMatcher->match(imageDesc1, imageDesc2, matches);
	std::cout << "total match points: " << matches.size() << std::endl;

	float max_dist = 0;
	float min_dist = 100;
	for (int i = 0; i < imageDesc1.rows; i++)
	{
		float dist = matches[i].distance;
		if (dist < min_dist)
			min_dist = dist;
		if (dist > max_dist)
			max_dist = dist;
	}
	std::cout << "Max min Distances:  " << max_dist << "," << min_dist << std::endl;
	for (int i = 0; i < imageDesc1.rows; i++) {
		if (matches[i].distance <= MAX(thresh_ratio * min_dist, 0.02)) {
			GoodMatches.push_back(matches[i]);
		}
	}
	cv::Mat imgMatches;
	cv::drawMatches(image1, keyPoint1, image2, keyPoint2, GoodMatches, imgMatches);
	cv::imwrite("match.jpg", imgMatches);

	pt_left.resize(GoodMatches.size());
	pt_right.resize(GoodMatches.size());
	for (int i = 0; i<GoodMatches.size(); i++){
		pt_left[i]=keyPoint2[GoodMatches[i].queryIdx].pt;
		pt_right[i]=keyPoint1[GoodMatches[i].trainIdx].pt;
	}
}
