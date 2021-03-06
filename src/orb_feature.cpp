#include "orb_feature.h"

OrbFeature::OrbFeature(){
	
}

OrbFeature::~OrbFeature(){

}

void OrbFeature::getMatches(std::vector<cv::Point2f>& pt_left, std::vector<cv::Point2f>& pt_right) {
	//left: query
	//right: train
	std::vector<cv::KeyPoint> keyPoint1, keyPoint2;  
	float scale1, scale2;
	int h1=image1.rows, h2=image2.rows;
    imresize(image1, height);
	imresize(image2, height);
	scale1=1.0*h1/height;
	scale2=1.0*h2/height;

	vector<DMatch> matches=matchImage(image1, image2, keyPoint1, keyPoint2, top_n);
	
	int N=MIN(matches.size(), top_n);
    for(int i=0;i<N;++i){
		int qidx=matches[i].queryIdx;
		int tidx=matches[i].trainIdx;
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
	//cv::Mat match_img=DrawInlier(image1, image2, keyPoint1, keyPoint2, matches, 2);
	//cv::imshow("match", match_img);
}