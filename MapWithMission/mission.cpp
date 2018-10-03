#include "mission.h"


Mission::Mission()
{

}


std::vector<std::pair<cv::Rect, TYPE> > Mission::findBoundingBoxes(cv::Mat& rgbImg, cv::Mat& biImg, int min_area, int max_area){
    std::vector<std::pair<cv::Rect, TYPE> > boxes;
    IplImage ipl(biImg);
    CvMemStorage* pStorage = cvCreateMemStorage(0);
    CvSeq* pContour = NULL;
    cvFindContours(&ipl, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    for (; pContour; pContour = pContour->h_next) {
        float true_area = fabs(cvContourArea(pContour));
        cv::Rect bbox = cvBoundingRect(pContour, 0);
        float box_area = 1.0*bbox.height*bbox.width;
        if (bbox.width > rgbImg.cols / 2 || bbox.height > rgbImg.rows / 2) {
            cvSeqRemove(pContour, 0);
            continue;
        }
        if (box_area > max_area || box_area < min_area) {
            cvSeqRemove(pContour, 0);
            continue;
        }
        if (box_area > 4*true_area) {
            cvSeqRemove(pContour, 0);
            continue;
        }
        int barrel_g=0, barrel_b=0, barrel_r=0;
        int croco_g=0, croco_b=0, croco_r=0;
        cv::Mat patch=rgbImg(bbox);
        uchar* data=patch.data;
        int size=bbox.width*bbox.height;
        for(int i=0;i<size;++i){
            int ind=i*3;
            int b=data[ind];
            int g=data[ind+1];
            int r=data[ind+2];
            if(b>barrel_thresh && b>g+barrel_diff_thresh && b>r+barrel_diff_thresh)
                barrel_b++;
            if(g>barrel_thresh && g>b+barrel_diff_thresh && g>r+barrel_diff_thresh)
                barrel_g++;
            if(r>barrel_thresh && r>b+barrel_diff_thresh && r>g+barrel_diff_thresh)
                barrel_r++;
            if(b>croco_thresh && b>g+croco_diff_thresh && b>r+croco_diff_thresh)
                croco_b++;
            if(g>croco_thresh && g>b+croco_diff_thresh && g>r+croco_diff_thresh)
                croco_g++;
            if(r>croco_thresh && r>b+croco_diff_thresh && r>g+croco_diff_thresh)
                croco_r++;
        }
        std::tuple<int,int,int> barrel_count=std::tuple<int,int,int>(barrel_b, barrel_g, barrel_r);
        //std::tuple<int,int,int> croco_count=std::tuple<int,int,int>(croco_b, croco_g, croco_r);
        if(isBarrel(barrel_count, bbox)){
            boxes.push_back(std::pair<cv::Rect, TYPE>(bbox, BARREL));
        }
        //else if(isCrocodile(croco_count, bbox)){
        //	boxes.push_back(std::pair<cv::Rect, TYPE>(bbox, CROCODILE));
        //}
    }
    cvReleaseMemStorage(&pStorage);
    return boxes;
}


bool Mission::isBarrel(std::tuple<int,int,int>& count, cv::Rect& rect, float min_ratio, float max_ratio){	//ratio:w/h
    //red
    if(rect.width<min_ratio*rect.height || rect.width>max_ratio*rect.height)
        return false;
    const int cnt_thresh=MIN(rect.width, rect.height);
    int b_cnt=std::get<0>(count);
    int g_cnt=std::get<1>(count);
    int r_cnt=std::get<2>(count);

    if(r_cnt>cnt_thresh && r_cnt>g_cnt && r_cnt>b_cnt){
        std::cout<<"Barrel: "<<b_cnt<<','<<g_cnt<<','<<r_cnt<<std::endl;
        return true;
    }
    return false;
}

bool Mission::isCrocodile(std::tuple<int,int,int>& count, cv::Rect& rect, float min_ratio, float max_ratio){
    //green-->white
    if(rect.width<min_ratio*rect.height || rect.width>max_ratio*rect.height)
        return false;
    const int cnt_thresh=MIN(rect.width, rect.height);

    int r_cnt=std::get<0>(count);
    int g_cnt=std::get<1>(count);
    int b_cnt=std::get<2>(count);

    if(g_cnt>cnt_thresh && g_cnt>r_cnt && g_cnt>b_cnt){
        std::cout<<"Crocodile: "<<b_cnt<<','<<g_cnt<<','<<r_cnt<<std::endl;
        return true;
    }
    return false;
}

void Mission::compareTargets(std::vector<std::pair<cv::Rect, TYPE> >& locations, \
                    std::vector<Target>& targets, \
                    const char* mode, \
                    const float overlap, \
                    const float eps, \
                    const int forgetFrame)
{
    int nLocations=locations.size();
    std::vector<int> matched(nLocations, 0);

    auto iter=targets.begin();

    for(;iter!=targets.end();){
        if(iter->forgot>=forgetFrame){
            iter=targets.erase(iter);
            iter->id--;
        }
        else
            ++iter;
    }
    int nTargets=targets.size();
    //std::cout<<"current targets: "<<targets.size()<<std::endl;
    for(int i=0;i<nTargets;++i){
        Target& target=targets[i];
        cv::Rect& t_loc=target.location;
        int tx=t_loc.x, ty=t_loc.y;
        int tw=t_loc.width, th=t_loc.height;
        float min_dist=1e7;
        int min_ind=0;
        float max_iou=0;
        int max_ind=0;
        for(int j=0;j<nLocations;++j){
            cv::Rect& loc=locations[i].first;
            TYPE type=locations[i].second;
            int lx=loc.x, ly=loc.y;
            int lw=loc.width, lh=loc.height;
            if(strcmp(mode, "dist")==0){
                float diff_x=tx-lx, diff_y=ty-ly;
                float dist=sqrt(diff_x*diff_x+diff_y*diff_y);
                if(dist<min_dist && type==target.type){
                    min_dist=dist;
                    min_ind=j;
                    if(dist<eps)
                        matched[j]=1;
                }
            }
            else if(strcmp(mode, "iou")==0){
                int rbx=MIN(tx+tw, lx+lw), rby=MIN(ty+th, ly+lh);
                int ltx=MAX(tx, lx), lty=MAX(ty, ly);
                int overlap_w=MAX(0, rbx-ltx);
                int overlap_h=MAX(0, rby-lty);
                int t_area=tw*th, l_area=lw*lh;
                float overlap_area=overlap_w*overlap_h;
                float iou=overlap_area/(t_area+l_area-overlap_area);
                if(iou>max_iou && type==target.type){
                    max_iou=iou;
                    max_ind=j;
                    if(iou>overlap)
                        matched[j]=1;
                }
            }
            else
                assert(0);
        }
        if(strcmp(mode, "dist")==0){
            if(min_dist<eps){
                target.location=locations[min_ind].first;
                target.life++;
                target.forgot=0;
            }
            else
                target.forgot++;
        }
        else if(strcmp(mode, "iou")==0){
            if(max_iou>overlap){
                target.location=locations[max_ind].first;
                target.life++;
                target.forgot=0;
            }
            else
                target.forgot++;
        }
        else
            assert(0);
    }

    GPS currentGps{0,0};
    if(targets.size()>0){
        currentGps=targets[0].gpsLocation;
    }
    for(int i=0;i<nLocations;++i){
        if(!matched[i]){
            int max_id=targets.size()>0?targets[nTargets-1].id:0;
            targets.push_back(Target{currentGps, locations[i].first, max_id, 0, 0, locations[i].second});
        }
    }
}

void Mission::compareTargetsWithGps(std::vector<std::pair<cv::Rect, TYPE> >& locations, \
                    std::vector<Target>& targets, \
                    GPS currentGps, \
                    const char* mode, \
                    const float overlap, \
                    const float eps, \
                    const int forgetFrame)
{
    int nLocations=locations.size();
    std::vector<int> matched(nLocations, 0);

    auto iter=targets.begin();

    for(;iter!=targets.end();){
        if(iter->forgot>=forgetFrame){
            iter=targets.erase(iter);
            iter->id--;
        }
        else
            ++iter;
    }
    int nTargets=targets.size();
    //std::cout<<"current targets: "<<targets.size()<<std::endl;
    for(int i=0;i<nTargets;++i){
        Target& target=targets[i];
        cv::Rect& t_loc=target.location;
        int tx=t_loc.x, ty=t_loc.y;
        int tw=t_loc.width, th=t_loc.height;
        float min_dist=1e7;
        int min_ind=0;
        float max_iou=0;
        int max_ind=0;

        for(int j=0;j<nLocations;++j){
            cv::Rect& loc=locations[i].first;
            TYPE type=locations[i].second;
            int lx=loc.x, ly=loc.y;
            int lw=loc.width, lh=loc.height;
            if(strcmp(mode, "dist")==0){
                float diff_x=tx-lx, diff_y=ty-ly;
                float dist=sqrt(diff_x*diff_x+diff_y*diff_y);
                if(dist<min_dist && type==target.type){
                    min_dist=dist;
                    min_ind=j;
                    if(dist<eps)
                        matched[j]=1;
                }
            }
            else if(strcmp(mode, "iou")==0){
                int rbx=MIN(tx+tw, lx+lw), rby=MIN(ty+th, ly+lh);
                int ltx=MAX(tx, lx), lty=MAX(ty, ly);
                int overlap_w=MAX(0, rbx-ltx);
                int overlap_h=MAX(0, rby-lty);
                int t_area=tw*th, l_area=lw*lh;
                float overlap_area=overlap_w*overlap_h;
                float iou=overlap_area/(t_area+l_area-overlap_area);
                if(iou>max_iou && type==target.type){
                    max_iou=iou;
                    max_ind=j;
                    if(iou>overlap)
                        matched[j]=1;
                }
            }
            else
                assert(0);
        }
        if(strcmp(mode, "dist")==0){
            if(min_dist<eps){
                target.location=locations[min_ind].first;
                target.life++;
                target.forgot=0;
            }
            else
                target.forgot++;
        }
        else if(strcmp(mode, "iou")==0){
            if(max_iou>overlap){
                target.location=locations[max_ind].first;
                target.life++;
                target.forgot=0;
            }
            else
                target.forgot++;
        }
        else
            assert(0);
    }
    for(int i=0;i<nLocations;++i){
        if(!matched[i]){
            int max_id=targets.size()>0?targets[nTargets-1].id:0;
            targets.push_back(Target{currentGps, locations[i].first, max_id, 0, 0, locations[i].second});
        }
    }
}

std::vector<std::pair<cv::Rect, TYPE> > Mission::findTargets(cv::Mat& oriImg){
    cv::Mat salientMap=salientDetect(oriImg);
    //cv::Mat biImg=adaBinarize(salientMap);
    cv::Mat biImg;
    cv::threshold(salientMap, biImg, 180, 255, cv::THRESH_OTSU);
    cv::imshow("bi", biImg);
    auto locations=findBoundingBoxes(oriImg, biImg, 100, 400000);
    return locations;
}
