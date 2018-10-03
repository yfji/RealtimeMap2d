#include <opencv2/opencv.hpp>
#include <vector>

float gamma(float x);
	
void RGBToLab(unsigned char * rgbImg, float * labImg);

cv::Mat salientDetect(cv::Mat& image);

cv::Mat adaBinarize(cv::Mat& salientMap);