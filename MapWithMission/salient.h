#include <opencv2/opencv.hpp>
#include <vector>
#include <thread>
float gamma(float x);
	
void RGBToLab(unsigned char * rgbImg, float * labImg);

cv::Mat salientDetect(cv::Mat& image);

void RGB2LabKernel(uchar* src_ptr, float* dst_ptr, int* size, std::vector<float>* means);

cv::Mat salientDetectFaster(cv::Mat& image, const int num_threads=12);

cv::Mat adaBinarize(cv::Mat& salientMap);
