#include "stitcher.h"
#include <fstream>

cv::Mat alignImage(cv::Mat& img, const cv::Size& size);

int main()
{
	const int im_w=640;
	const int im_h=360;
	const int preview_w=360;
	const int preview_h=360;

	std::ifstream in;
	in.open("file_lst.txt", std::ios::in);
	std::vector<std::string> img_paths;
	while(!in.eof()){
		std::string line;
		std::getline(in, line);
		if(line.length()==0){
			continue;
		}
		img_paths.push_back(line);
	}
	in.close();
	// std::vector<std::string> img_paths = { "./rgb/1455008966.000000.jpg",\
	// 	"./rgb/1455008967.600000.jpg",\
	// 	"./rgb/1455008968.880000.jpg",\
	// 	"./rgb/1455008971.680000.jpg" };
	ImageStitcher stitcher(im_w, im_h);

	char key=0;
	//cv::VideoWriter writer("map.avi", CV_FOURCC('M','J','P','G'), 3, cv::Size(preview_w, preview_h));
	for(int i=0;i<img_paths.size();++i){
		std::cout<<img_paths[i]<<std::endl;
		cv::Mat image=cv::imread(img_paths[i]);
		cv::resize(image, image, cv::Size(im_w, im_h), cv::INTER_LINEAR);
		stitcher.stitch(image);
		const cv::Mat& map = stitcher.getStitchedImage();
		cv::Mat previewMap=map.clone();
		imresize(previewMap, preview_h);
		cv::imshow("map", previewMap);
		cv::Mat w_image=alignImage(previewMap, cv::Size(preview_w, preview_h));
		//writer<<w_image;
		key=cv::waitKey(1);
		if(key==27){
			break;
		}
		else if(key==' '){
			cv::waitKey();
		}
	}

    return 0;
}

cv::Mat alignImage(cv::Mat& img, const cv::Size& size){
	cv::Mat canvas=cv::Mat::zeros(size.height, size.width, CV_8UC3);
	int start_y=MAX(0,std::floor((size.height-img.rows)/2));
	int start_x=MAX(0,std::floor((size.width-img.cols)/2));
	cv::Mat img_part=img(cv::Rect(0,0,MIN(img.cols, size.width), MIN(img.rows, size.height)));
	img_part.copyTo(canvas(cv::Rect(start_x, start_y, MIN(img.cols, size.width), MIN(img.rows, size.height))));
	return canvas;
}