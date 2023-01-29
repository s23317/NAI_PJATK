cmake_minimum_required(VERSION 3.23)
project(ocvdemo)

set(CMAKE_CXX_STANDARD 17)

add_executable(ocvdemo cameraCalibration.cpp)

set(OpenCV_DIR "C:\\Users\\antek\\Documents\\NAI\\opencv\\opencv-4.6.0\\cmake-build-release")

find_package(OpenCV REQUIRED)

if (OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    link_directories(${OpenCV_LIB_DIR})
    target_link_libraries(ocvdemo opencv_core;opencv_highgui;opencv_imgcodecs;opencv_imgproc;opencv_videoio;opencv_aruco)
endif ()
