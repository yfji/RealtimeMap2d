#ifndef _FEATURE_H_
#define _FEATURE_H_
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

class Feature
{
public:
	Feature();
	~Feature();

	cv::Mat image1;
	cv::Mat image2;

	virtual void getMatches(std::vector<cv::Point2f>& pt_left, std::vector<cv::Point2f>& pt_right) = 0;
	inline void setAlignHeight(int h){
		height=h;
	}
	inline void setMatchNumber(int n){
		top_n=n;
	}

    bool showMatchImage= {false};
protected:
	int height={480};
	int top_n={50};
};
#endif

