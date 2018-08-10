#include "stitcher.h"
#include <exception>

ImageStitcher::ImageStitcher(int w, int h)
{
	width = w;
	height = h;
	//ptrFeature.reset(new SurfFeature());
	ptrFeature.reset(new OrbFeature());
	ptrFeature->setAlignHeight(height);
	ptrFeature->setMatchNumber(800);

	dist_max=sqrt(pad_x*pad_x+pad_y*pad_y);
	match_center=cv::Point2i(0,0);
	box_area=cv::Rect(0,0,0,0);
	offset=cv::Point2i(0,0);
	max_roi_offset=cv::Point2i(0,0);
	min_roi_offset=cv::Point2i(1e4,1e4);
}

ImageStitcher::~ImageStitcher()
{
}

cv::Mat ImageStitcher::getPatch() {
	if (firstImg) {
		firstImg = 0;
		return map2d;
	}
	int start_x = MAX(0, MIN(map2d.cols - width, match_center.x - width / 2 - 1));
	int start_y = MAX(0, MIN(map2d.rows - height, match_center.y - height / 2 - 1));
	speed_x=start_x-offset.x;
	speed_y=start_y-offset.y;
	//cv::line(map2d, cv::Point2i(offset.x,offset.y),cv::Point2i(start_x, start_y), cv::Scalar(0,0,255),1);
	offset.x = start_x;
	offset.y = start_y;
	cv::Mat patch=map2d(cv::Rect(start_x, start_y, width, height));
	return patch;
}

void ImageStitcher::checkIfStitchable(){
	float corner_dist=0.;
	int std_corner_x[4]={pad_x,pad_x,width+pad_x,width+pad_x};
	int std_corner_y[4]={pad_y,height+pad_y,pad_y,height+pad_y};
	for(int i=0;i<4;++i){
		cv::Point2f& c=corners[i];
		float d1=c.x-std_corner_x[i];
		float d2=c.y-std_corner_y[i];
		float dist=sqrt(d1*d1+d2*d2);
		if(dist>corner_dist){
			corner_dist=dist;
		}
	}
	float eps=corner_dist/dist_max;
	if(eps>0.333){
		ignore=1;
	}
}

void ImageStitcher::calcWarpCorners(cv::Mat& warpMatrix) {
	//corners: left_top, left_bottom, right_top, right_bottom
	float mat[12] = {
		pad_x,pad_y,1,pad_x,height+pad_y,1,width+pad_x,pad_y,1,width+pad_x,height+pad_y,1
	};
	cv::Mat coords(4, 3, CV_32F, mat);
	coords=coords.t();	//3x4
	//std::cout<<warpMatrix.cols<<","<<warpMatrix.rows<<std::endl;
	cv::Mat warpCoords = warpMatrix*coords;	//3x4
	warpCoords = warpCoords.t();
	float* data=nullptr;
	try{
		data = (float*)warpCoords.data;
	}
	catch(cv::Exception& e){
		std::cout<<"Matrix error"<<std::endl;
		exit(0);
	}
	corners[0] = cv::Point2f(data[0] / data[2], data[1] / data[2]);
	corners[1] = cv::Point2f(data[3] / data[5], data[4] / data[5]);
	corners[2] = cv::Point2f(data[6] / data[8], data[7] / data[8]);
	corners[3] = cv::Point2f(data[9] / data[11], data[10] / data[11]);	//scale
	int max_x=0, max_y=0, min_x=1e5, min_y=1e5;
	for(int i=0;i<4;++i){
		cv::Point2f& c=corners[i];
		c.x=MIN(2*pad_x+width, MAX(c.x,0));
		c.y=MIN(2*pad_y+height, MAX(c.y, 0));
		max_x=MAX(max_x, c.x);
		max_y=MAX(max_y, c.y);
		min_x=MIN(min_x, c.x);
		min_y=MIN(min_y, c.y);
	}
	box_area.x=MIN(pad_x, min_x);
	box_area.y=MIN(pad_y, min_y);
	box_area.width=MAX(pad_x+width,max_x+1)-box_area.x;
	box_area.height=MAX(pad_y+height,max_y+1)-box_area.y;
}

void ImageStitcher::optimize(cv::Mat& patch, cv::Mat& ref, cv::Mat& trans) {
	int start_x=MAX(pad_x,MIN(corners[0].x, corners[1].x));
	int start_y=MAX(pad_y,MIN(corners[0].y, corners[2].y));
	int end_x=MIN(pad_x+width,MAX(corners[2].x, corners[3].x));
	int end_y=MIN(pad_y+height,MAX(corners[1].y, corners[3].y));
	// int start_x=box_area.x;
	// int start_y=box_area.y;
	// int end_x=box_area.x+box_area.width;
	// int end_y=box_area.y+box_area.height;

	int W=end_x-start_x+1;
	int H=end_y-start_y+1;

	int pad=0;
	float _alpha=0.6;
	
	for(int i=start_y-pad;i<end_y+pad;++i){
		uchar* patch_data=patch.ptr(i);
		uchar* ref_data=ref.ptr(i);
		uchar* trans_data=trans.ptr(i);
		for(int j=start_x-pad;j<end_x+pad;++j){
			int ind=j*3;
			float alpha=0.;
			if(ref_data[ind]<=pix_thresh && ref_data[ind+1]<=pix_thresh && ref_data[ind+2]<=pix_thresh){
				alpha=1.0;
			}
			else if(patch_data[ind]<=pix_thresh && patch_data[ind+1]<=pix_thresh && patch_data[ind+2]<=pix_thresh){	
				alpha=0.0;
			}
			else{
				alpha=_alpha;	
			}
			trans_data[ind]=(int)(alpha*patch_data[ind]+(1.-alpha)*ref_data[ind]);
			trans_data[ind+1]=(int)(alpha*patch_data[ind+1]+(1.-alpha)*ref_data[ind+1]);
			trans_data[ind+2]=(int)(alpha*patch_data[ind+2]+(1.-alpha)*ref_data[ind+2]);
		}
	}
	//cv::imshow("stitch", trans);
	//cv::rectangle(trans, cv::Rect(start_x, start_y, W,H), cv::Scalar(0,0,255), 2);
}

void ImageStitcher::applyOffset(){
	cv::Mat roi=stitchImage(box_area);
	
	//cv::imshow("roi", roi);
	if(map2d.empty()){
		map2d=roi.clone();
	}
	else{
		int min_x=box_area.x;
		int min_y=box_area.y;

		int map_offset_x=MAX(0,pad_x-min_x-offset.x);
		int map_offset_y=MAX(0,pad_y-min_y-offset.y);

		// int roi_offset_x=MAX(0,offset.x+min_x-pad_x);
		// int roi_offset_y=MAX(0,offset.y+min_y-pad_y);
		int roi_offset_x=MAX(0,offset.x+MIN(0,min_x-pad_x));
		int roi_offset_y=MAX(0,offset.y+MIN(0,min_y-pad_y));

		// int rb_offset_x=MAX(0,offset.x+min_x-pad_x+box_area.width-map2d.cols);
		// int rb_offset_y=MAX(0,offset.y+min_x-pad_x+box_area.height-map2d.rows);
		
		int rb_offset_x=MAX(0,roi_offset_x+box_area.width-map2d.cols-map_offset_x);
		int rb_offset_y=MAX(0,roi_offset_y+box_area.height-map2d.rows-map_offset_y);

		int nw=map2d.cols+map_offset_x+rb_offset_x+5;
		int nh=map2d.rows+map_offset_x+rb_offset_y+5;

		cv::Mat tmp_map=map2d.clone();
		cv::Mat new_map=cv::Mat::zeros(nh,nw,map2d.type());

		// std::cout<<nw<<","<<nh<<std::endl;
		// std::cout<<map_offset_x<<","<<map_offset_x<<","<<tmp_map.cols<<","<<tmp_map.rows<<std::endl;
		// std::cout<<rb_offset_x<<","<<rb_offset_y<<std::endl;
		// std::cout<<roi_offset_x<<","<<roi_offset_y<<","<<roi.cols<<","<<roi.rows<<std::endl;

		tmp_map.copyTo(new_map(cv::Rect(map_offset_x, map_offset_x, tmp_map.cols, tmp_map.rows)));

		float _alpha;
		if((roi_offset_x<min_roi_offset.x && roi_offset_y<min_roi_offset.y) || (roi_offset_x>max_roi_offset.x && roi_offset_y>max_roi_offset.y)){
			_alpha=0.3;
			std::cout<<"Trail blazer!"<<std::endl;
		}
		else{
			std::cout<<"Repaint old land!"<<std::endl;
			_alpha=0.92;
		}
		max_roi_offset.x=MAX(max_roi_offset.x, roi_offset_x);
		max_roi_offset.y=MAX(max_roi_offset.y, roi_offset_y);
		min_roi_offset.x=MIN(min_roi_offset.x, roi_offset_x);
		min_roi_offset.y=MIN(min_roi_offset.y, roi_offset_y);

		for(int i=roi_offset_y;i<roi_offset_y+roi.rows;++i){
			uchar* roi_data=roi.ptr(i-roi_offset_y);
			uchar* map_data=new_map.ptr(i);
			for(int j=roi_offset_x;j<roi_offset_x+roi.cols;++j){
				int ind=j*3;
				int roi_ind=(j-roi_offset_x)*3;
				float alpha=0.;
				if(roi_data[roi_ind]<=pix_thresh && roi_data[roi_ind+1]<=pix_thresh && roi_data[roi_ind+2]<=pix_thresh){
				 	alpha=1.;
				}
				else if(map_data[ind]<=pix_thresh && map_data[ind+1]<=pix_thresh && map_data[ind+2]<=pix_thresh){
					alpha=0.;
				}
				else{
					alpha=_alpha;
				}
				map_data[ind]=(int)(alpha*map_data[ind]+(1-alpha)*roi_data[roi_ind]);
				map_data[ind+1]=(int)(alpha*map_data[ind+1]+(1-alpha)*roi_data[roi_ind+1]);
				map_data[ind+2]=(int)(alpha*map_data[ind+2]+(1-alpha)*roi_data[roi_ind+2]);
			}
		}

		// roi.copyTo(new_map(cv::Rect(roi_offset_x, roi_offset_y, roi.cols, roi.rows)));		
		map2d=new_map.clone();
		
		offset.x+=map_offset_x;
		offset.y+=map_offset_y;

		match_center.x+=map_offset_x;
		match_center.y+=map_offset_y;
	}
}

void ImageStitcher::stitch(cv::Mat& img) {
	if (map2d.empty()) {
		map2d=img.clone();
		return;
	}
	std::vector<cv::Point2f> pt_left, pt_right;
	cv::Mat patch = getPatch();

	cv::Mat canvas_img=cv::Mat::zeros(img.rows+2*pad_y, img.cols+2*pad_x, img.type());
	cv::Mat canvas_patch=cv::Mat::zeros(patch.rows+2*pad_y, patch.cols+2*pad_x, patch.type());
	img.copyTo(canvas_img(cv::Rect(pad_x, pad_y, img.cols, img.rows)));
	patch.copyTo(canvas_patch(cv::Rect(pad_x, pad_y, patch.cols, patch.rows)));

	ptrFeature->image1 = patch;
	ptrFeature->image2 = img;	

	ptrFeature->getMatches(pt_left, pt_right);

	float c_x=0, c_y=0;
	for(auto i=0;i<pt_left.size();++i){
		c_x+=pt_left[i].x;
		c_y+=pt_left[i].y;
		pt_left[i].x+=pad_x;
		pt_left[i].y+=pad_y;
		pt_right[i].x+=pad_x;
		pt_right[i].y+=pad_y;
	}
	match_center.x=(int)(c_x/pt_left.size());
	match_center.y=(int)(c_y/pt_left.size());
	match_center.x+=offset.x;
	match_center.y+=offset.y;

	if(pt_left.size()>MIN_MATCH_SIZE){
		cv::Mat warpMatrix = cv::findHomography(pt_right, pt_left, CV_RANSAC);
		if(warpMatrix.cols==0 || warpMatrix.rows==0){
			std::cout<<"Warp matrix error!"<<std::endl;
			return;
		}
		warpMatrix.convertTo(warpMatrix, CV_32F, 1.0);
		calcWarpCorners(warpMatrix);
		checkIfStitchable();
		if(!ignore){
			//canvas_img, canvas_patch have the same size if using camera
			stitch_size.width=MAX(canvas_img.cols, canvas_patch.cols);
			stitch_size.height=MAX(canvas_img.rows, canvas_patch.rows);

			warpPerspective(canvas_img, stitchImage, warpMatrix, stitch_size);
			cv::Mat refImage=stitchImage.clone();
			//patch.copyTo(stitchImage(cv::Rect(pad_x, pad_y, patch.cols, patch.rows)));
			optimize(canvas_patch, refImage, stitchImage);
			//cv::imshow("warp", stitchImage);
			//cv::waitKey();
			applyOffset();
		}
		else{
			std::cout<<"Frame ignored"<<std::endl;
			ignore=0;
			//match_center.x+=speed_x/8;
			//match_center.y+=speed_y/8;
		}
	}
	else{
		std::cout<<"No matching points"<<std::endl;
	}
}