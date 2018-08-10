#include "surf_feature.h"


SurfFeature::SurfFeature()
{
	ptrDetector= cv::xfeatures2d::SURF::create(minHessian);
}


SurfFeature::~SurfFeature()
{
}

void SurfFeature::getMatches(std::vector<cv::Point2f>& pt_left, std::vector<cv::Point2f>& pt_right) {
	std::vector<cv::KeyPoint> keyPoint1, keyPoint2;  
	float scale1, scale2;
	int h1=image1.rows, h2=image2.rows;
    imresize(image1, height);
	imresize(image2, height);
	scale1=1.0*h1/height;
	scale2=1.0*h2/height;
	
	cv::Mat d1,d2;
	//ptrDetector->detect(image1, keyPoint1, cv::Mat());
	//ptrDetector->detect(image2, keyPoint2, cv::Mat());
	ptrDetector->detectAndCompute(image1, cv::Mat(), keyPoint1, d1);
	ptrDetector->detectAndCompute(image2, cv::Mat(), keyPoint2, d2);
	//ptrExtractor->compute(image1, keyPoint1, imageDesc1);
	//ptrExtractor->compute(image2, keyPoint2, imageDesc2);

	std::vector<cv::DMatch> matches;
	std::vector<cv::DMatch> GoodMatches;

	cv::FlannBasedMatcher matcher;

	matcher.match(d1, d2, matches);
	std::cout << "total match points: " << matches.size() << std::endl;
	std::sort(matches.begin(), matches.end(), [](cv::DMatch& m1, cv::DMatch& m2) {
		return (m1.distance < m2.distance);
	});

	int N=MIN(matches.size(), top_n);
	for(int i=0;i<N;++i){
		GoodMatches.push_back(matches[i]);
	}
	//cv::Mat imgMatches;
	//cv::drawMatches(image1, keyPoint1, image2, keyPoint2, GoodMatches, imgMatches);
	//cv::imwrite("match.jpg", imgMatches);

	for(int i=0;i<N;++i){
		int qidx=GoodMatches[i].queryIdx;
		int tidx=GoodMatches[i].trainIdx;
		if(qidx>keyPoint1.size()){
			std::cout<<"Left overbound: ("<<qidx<<","<<keyPoint1.size()<<")"<<std::endl;
		}
		if(tidx>keyPoint2.size()){
			std::cout<<"Right overbound: ("<<tidx<<","<<keyPoint2.size()<<")"<<std::endl;
		}
		if(qidx<0 || qidx>=keyPoint1.size() || tidx<0 || tidx>keyPoint2.size()){
			continue;
		}
		cv::Point2f& l_pt=keyPoint1[qidx].pt;
		cv::Point2f& r_pt=keyPoint2[tidx].pt;
		pt_left.push_back(cv::Point2f(l_pt.x*scale1, l_pt.y*scale1));
		pt_right.push_back(cv::Point2f(r_pt.x*scale2, r_pt.y*scale2));
	}
}
