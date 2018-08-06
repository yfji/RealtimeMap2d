#include "stitcher.h"
#include <fstream>

int main()
{
	const int im_w=640;
	const int im_h=360;
	const int preview_w=720;
	const int preview_h=720;

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
	for(int i=0;i<img_paths.size();++i){
		std::cout<<img_paths[i]<<std::endl;
		cv::Mat image=cv::imread(img_paths[i]);
		cv::resize(image, image, cv::Size(im_w, im_h), cv::INTER_LINEAR);
		stitcher.stitch(image);
		const cv::Mat& map = stitcher.getStitchedImage();
		cv::Mat previewMap=map.clone();
		imresize(previewMap, preview_h);
		cv::imshow("map", previewMap);
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

