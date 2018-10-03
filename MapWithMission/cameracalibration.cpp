#include "cameracalibration.h"

CameraCalibration::CameraCalibration()
{
}


CameraCalibration::~CameraCalibration()
{
}

void CameraCalibration::initMatrix(){
	innerMatrix = cv::Mat::zeros(cv::Size(3, 3), CV_32FC1);
	distortCoeffMatrix = cv::Mat::zeros(cv::Size(5,1), CV_32FC1);

#ifdef _1080P
    string mat_file="/home/yfji/Workspace/Qt/Map2d/build-Map2d-Desktop_Qt_5_10_1_GCC_64bit-Release/matrix_540.txt";
#else
    string mat_file="/home/yfji/Workspace/Qt/Map2d/build-Map2d-Desktop_Qt_5_10_1_GCC_64bit-Release/matrix_540.txt";
#endif
	ifstream in;
	in.open(mat_file.c_str(), ios::in);
	if(in){
        std::cout<<"Loading inner matrix"<<std::endl;
		for(int i=0;i<3;++i){
			float* ptr=innerMatrix.ptr<float>(i);
			in>>ptr[0]>>ptr[1]>>ptr[2];
		}
		float* ptr=distortCoeffMatrix.ptr<float>(0);
		for(int j=0;j<5;++j)
			in>>ptr[j];
	}
	else{
		innerMatrix.at<float>(0, 0)=590.974;
		innerMatrix.at<float>(0, 1)=0;
		innerMatrix.at<float>(0, 2)=474.396;
		innerMatrix.at<float>(1, 0)=0;
		innerMatrix.at<float>(1, 1)=701.05;
		innerMatrix.at<float>(1, 2)=289.976;
		innerMatrix.at<float>(2, 0)=0;
		innerMatrix.at<float>(2, 1)=0;
		innerMatrix.at<float>(2, 2)=1;

		distortCoeffMatrix.at<float>(0, 0)=-0.347977;
		distortCoeffMatrix.at<float>(0, 1)=0.125386;
		distortCoeffMatrix.at<float>(0, 2)=-0.000144567;
		distortCoeffMatrix.at<float>(0, 3)= 0.000318595;
		distortCoeffMatrix.at<float>(0, 4)= 0.0725469;
	}
}

cv::Point2f CameraCalibration::calcPoint(float x, float y) {

	float ux = innerMatrix.at<float>(0, 2);
	float uy = innerMatrix.at<float>(1, 2);
	float fx = innerMatrix.at<float>(0, 0);
	float fy = innerMatrix.at<float>(1, 1);
	float k1 = distortCoeffMatrix.at<float>(0, 0);
	float k2 = distortCoeffMatrix.at<float>(0, 1);
	float p1 = distortCoeffMatrix.at<float>(0, 2);
	float p2 = distortCoeffMatrix.at<float>(0, 3);
	float k3 = distortCoeffMatrix.at<float>(0, 4);
	

	float xCorrected = (x - ux) / fx;
	float yCorrected = (y - uy) / fy;

	float xDistortion, yDistortion;

	float r2 = xCorrected * xCorrected + yCorrected * yCorrected;

	float deltaRa = 1. + k1 * r2 + k2 * r2 * r2 +k3 * r2 * r2 * r2;
	float deltaRb = 1.0;// /(1.0+k4 * r2 + k5 * r2 * r2 + k6 * r2 * r2 * r2);

	float deltaTx =  2. * p1 * xCorrected * yCorrected + p2 * (r2 + 2. * xCorrected * xCorrected);
	float deltaTy =  p1 * (r2 + 2. * yCorrected * yCorrected) + 2. * p2 * xCorrected * yCorrected;

	xDistortion = xCorrected * deltaRa*deltaRb+deltaTx;
	yDistortion = yCorrected * deltaRa*deltaRb+deltaTy;

	xDistortion = xDistortion * fx + ux;
	yDistortion = yDistortion * fy + uy;

	//cout<<xDistortion<<' '<<yDistortion<<endl;
	return cv::Point2f(xDistortion, yDistortion);
}

std::vector<int> CameraCalibration::interpolation(float rx, float ry, int w, int h, uchar* pData) {
	int x = MAX(0, rx);
	int y = MAX(0, ry);
	int y_low = (int)y;
	int x_low = (int)x;
	int y_high;
	int x_high;
	if (y_low >= h - 1) {
		y_high = y_low = h - 1;
		y = (float)y_low;
	}
	else {
		y_high = y_low + 1;
	}
	if (x_low >= w - 1) {
		x_high = x_low = w - 1;
		x = (float)x_low;
	}
	else {
		x_high = x_low + 1;
	}
	float ly = y - y_low;
	float lx = x - x_low;
	float hy = 1. - ly, hx = 1. - lx;
	float pos[4] = { y_low*w + x_low, y_low*w + x_high, y_high*w + x_low, y_high*w + x_high };
	float weight[4] = { hy*hx, hy*lx, ly*hx, ly*lx };

	std::vector<int> vals(3);
	for (int i = 0; i < 3; ++i) {
		float v = 0;
		for (int j = 0; j < 4; ++j) {
			int index = (int)(pos[j]);
			v += (int)(pData[3 * index + i]) * weight[j];
		}
		vals[i] = (int)v;
	}
	return vals;
}

void CameraCalibration::getInterpImage(cv::Mat& oriImg, cv::Mat& correctedImage) {
	int h = oriImg.rows, w = oriImg.cols, c = oriImg.channels();
	uchar* data = oriImg.data;

	cv::undistort(oriImg, correctedImage, innerMatrix, distortCoeffMatrix);
	// for (int i = 0; i < h; ++i) {
	// 	uchar* correctPtr = correctedImage.ptr(i);
	// 	for (int j = 0; j < w; ++j) {
	// 		int ind = j * c;
	// 		cv::Point2f np = calcPoint(1.0*j, 1.0*i);
	// 		std::vector<int> vals = interpolation(np.x, np.y, w, h, data);
	// 		//cout<<vals[0]<<' '<<vals[1]<<' '<<vals[2]<<':'<<i<<','<<j<<endl;
	// 		correctPtr[ind] = (uchar)vals[0];
	// 		correctPtr[ind + 1] = (uchar)vals[1];
	// 		correctPtr[ind + 2] = (uchar)vals[2];
	// 	}
	// }
}

void CameraCalibration::undistort(cv::Mat& oriImg, cv::Mat& correctedImage){
	cv::Mat mapx = cv::Mat(oriImg.size(), CV_32FC1);
    cv::Mat mapy = cv::Mat(oriImg.size(), CV_32FC1);
    cv::Mat R = cv::Mat::eye(3, 3, CV_32F);

	initUndistortRectifyMap(innerMatrix, distortCoeffMatrix, R, innerMatrix, oriImg.size(), CV_32FC1, mapx, mapy);
	
	cv::remap(oriImg, correctedImage, mapx, mapy, cv::INTER_LINEAR);     
}
