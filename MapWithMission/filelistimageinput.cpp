#include "filelistimageinput.h"
#include <string.h>

FileListImageInput::FileListImageInput(const std::string& s):
    BaseImageInput(s)
{
    type=FILELIST;
    fileList=s;
    loadFrames();
}

cv::Mat FileListImageInput::getRawImage(){
    cv::Mat image;
    if(opened && curFrameIndex<numFrames){
        std::string fileName=dir_path+fileNames[curFrameIndex];
        image=cv::imread(fileName);
        curFrameIndex+=1;
    }
    return image;
}

void FileListImageInput::loadFrames(){
    std::ifstream in;
    in.open(fileList.c_str(), std::ios::in);
    std::cout<<fileList<<std::endl;
    if(in.is_open()){
        if(fileNames.size()>0){
            std::vector<std::string>().swap(fileNames);
            fileNames.clear();
        }
        opened=true;
        char ok=0;
        while(!in.eof()){
            std::string line;
            std::getline(in, line);
            if(line.length()>0){
                int pos=line.find_last_of('/')+1;
                std::string tmp=line;
                if(!ok){
                    dir_path=tmp.replace(pos,tmp.length()-pos,"");
                    ok=1;
                }
                line=line.replace(0,pos,"");
                fileNames.push_back(line);
            }
        }
        std::sort(fileNames.begin(), fileNames.end(), [this](const std::string& s1, const std::string& s2){
            const char* c1=s1.c_str();
            const char* c2=s2.c_str();
            int res=strcmp(c1, c2);
            return (res<0);
        });
    }
    else{
        opened=false;
    }
    numFrames=fileNames.size();
    isFinite=true;
    //std::cout<<numFrames<<" Frames are loaded"<<std::endl;
    //std::cout<<"Dir path: "<<dir_path<<std::endl;
    //std::cout<<"First frame: "<<fileNames[0]<<std::endl;
}
