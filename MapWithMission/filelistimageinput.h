#ifndef FILELISTIMAGEINPUT_H
#define FILELISTIMAGEINPUT_H

#include "baseimageinput.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class FileListImageInput : public BaseImageInput
{
public:
    FileListImageInput(const std::string& s);

    virtual cv::Mat getRawImage();

private:
    std::string fileList;
    std::string dir_path;
    std::vector<std::string> fileNames;

    void loadFrames();
};

#endif // FILELISTIMAGEINPUT_H
