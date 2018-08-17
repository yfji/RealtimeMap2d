project(ImageStitch)

cmake_minimum_required(VERSION 2.6)
aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRCS)
add_definitions("-Wall -g -std=c++11 -fPIC")

option(USE_GPU "select to use gpu" OFF)
message("USE_GPU is ${USE_GPU}")

if(USE_GPU)
    add_definitions(-DUSE_GPU)
endif(USE_GPU)

include_directories(
	/usr/include/
	/usr/local/include
	/usr/local/cuda/include
	)

	
link_directories(
	/usr/lib
	/usr/local/lib
	/usr/lib/x86_64-linux-gnu
	/usr/local/cuda/lib64
	/home/yfji/Workspace/OpenCV/ImageStitch/lib
	)
	
	
add_executable(ImageStitch ${DIR_SRCS})
target_link_libraries(
	ImageStitch
	opencv_core
	opencv_highgui
	opencv_imgproc
	opencv_imgcodecs
	opencv_videoio
	opencv_xfeatures2d
	opencv_features2d
	opencv_xobjdetect
	opencv_objdetect
	opencv_flann
	opencv_calib3d
	gms
	)
