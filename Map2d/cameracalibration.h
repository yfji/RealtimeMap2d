#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class CameraCalibration
{
public:
	CameraCalibration();
	~CameraCalibration();

private:
	cv::Mat innerMatrix;
	cv::Mat extMatrix;
	cv::Mat distortCoeffMatrix;
	
public:
	void initMatrix();

    cv::Point2f calcPoint(float x, float y);
	std::vector<int> interpolation(float rx, float ry, int w, int h, uchar* pData);
	void getInterpImage(cv::Mat& oriImg, cv::Mat& correctedImage);
	void undistort(cv::Mat& oriImg, cv::Mat& correctedImage);
};

