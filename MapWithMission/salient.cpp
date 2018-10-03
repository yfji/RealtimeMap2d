#include "salient.h"

float gamma(float x) {
	return x>0.04045 ? pow((x + 0.055f) / 1.055f, 2.4f) : x / 12.92;
}

void RGBToLab(unsigned char * rgbImg, float * labImg) {
	float B = gamma(rgbImg[0] / 255.0f);
	float G = gamma(rgbImg[1] / 255.0f);
	float R = gamma(rgbImg[2] / 255.0f);
	float X = 0.412453*R + 0.357580*G + 0.180423*B;
	float Y = 0.212671*R + 0.715160*G + 0.072169*B;
	float Z = 0.019334*R + 0.119193*G + 0.950227*B;

	X /= 0.95047;
	Y /= 1.0;
	Z /= 1.08883;

	float FX = X > 0.008856f ? pow(X, 1.0f / 3.0f) : (7.787f * X + 0.137931f);
	float FY = Y > 0.008856f ? pow(Y, 1.0f / 3.0f) : (7.787f * Y + 0.137931f);
	float FZ = Z > 0.008856f ? pow(Z, 1.0f / 3.0f) : (7.787f * Z + 0.137931f);
	labImg[0] = Y > 0.008856f ? (116.0f * FY - 16.0f) : (903.3f * Y);
	labImg[1] = 500.f * (FX - FY);
	labImg[2] = 200.f * (FY - FZ);
}

cv::Mat salientDetect(cv::Mat& image){
	assert(image.channels() == 3);
	cv::Mat salMap(image.size(), CV_32FC1);
	cv::Mat lab, labf;
	int h = image.rows, w = image.cols;
	labf.create(cv::Size(w, h), CV_32FC3);
	uchar* fSrc = image.data;
	float* fLab = (float*)labf.data;
	float* fDst = (float*)salMap.data;

	int stride = w * 3;
	//for (int i = 0; i < h; ++i) {
	//	for (int j = 0; j < stride; j += 3) {
	//		RGBToLab(fSrc + i*stride + j, fLab + i*stride + j);
	//	}
	//}
	float MeanL = 0, MeanA = 0, MeanB = 0;
	for (int i = 0; i < h; ++i) {
		int index = i*stride;
		for (int x = 0; x < w; ++x) {
			RGBToLab(fSrc + index, fLab + index);
			MeanL += fLab[index];
			MeanA += fLab[index + 1];
			MeanB += fLab[index + 2];
			index += 3;
		}
	}
	MeanL /= (w * h);
	MeanA /= (w * h);
	MeanB /= (w * h);
	cv::GaussianBlur(labf, labf, cv::Size(5, 5), 1);
	for (int Y = 0; Y < h; Y++)
	{
		int Index = Y * stride;
		int CurIndex = Y * w;
		for (int X = 0; X < w; X++)
		{
			fDst[CurIndex++] = (MeanL - fLab[Index]) *  \
				(MeanL - fLab[Index]) + (MeanA - fLab[Index + 1]) *  \
				(MeanA - fLab[Index + 1]) + (MeanB - fLab[Index + 2]) *  \
				(MeanB - fLab[Index + 2]);
			Index += 3;
		}
	}
	cv::normalize(salMap, salMap, 0, 1, cv::NORM_MINMAX);
	salMap.convertTo(salMap, CV_8UC1, 255);
	//cv::imshow("salient", salMap);
	return salMap;
}

cv::Mat adaBinarize(cv::Mat& salientMap){
    assert(salientMap.channels() == 1);
	const int blockSize = 13;
	const int threshold = 11;
	const int sz = blockSize*blockSize;
	int halfSize = blockSize / 2;
	cv::Rect roi(halfSize, halfSize, salientMap.cols, salientMap.rows);
	cv::Mat borderImg;
	cv::copyMakeBorder(salientMap, borderImg, halfSize, halfSize, halfSize, halfSize, cv::BORDER_CONSTANT, 0);

	cv::Mat iimage, biMap;
	cv::integral(borderImg, iimage, CV_32S);
	biMap.create(borderImg.size(), CV_8UC1);
	for (int j = halfSize; j < borderImg.rows - halfSize - 1; ++j) {
		uchar* data = biMap.ptr(j);
		uchar* im_data = (uchar*)borderImg.ptr(j);
		int* idata1 = iimage.ptr<int>(j - halfSize);
		int* idata2 = iimage.ptr<int>(j + halfSize + 1);
		for (int i = halfSize; i < salientMap.cols - halfSize - 1; ++i) {
			int sum = (idata2[i + halfSize + 1] - idata2[i - halfSize] - idata1[i + halfSize + 1] + idata1[i - halfSize]);
			sum /= sz;
			if (im_data[i] < sum-threshold)
				data[i] = 0;
			else
				data[i] = 255;
		}
	}
	cv::Mat biMapNot;
	cv::bitwise_not(biMap(roi), biMapNot);
	return biMapNot;
	//return biMap(roi).clone();
}