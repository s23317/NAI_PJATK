#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/aruco.hpp"
#include "opencv2/calib3d.hpp"

#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <list>
#include <random>

using namespace std;
using namespace cv;
std::random_device rd;
std::mt19937 generator(rd());

const float arucoSquareDimension = 0.1016f;

bool loadCameraCalibration(string name, Mat &cameraMatrix, Mat &distanceCoefficients) {
    ifstream inStream(name);
    if (inStream) {
        uint16_t rows;
        uint16_t columns;

        inStream >> rows;
        inStream >> columns;

        cameraMatrix = Mat(Size(columns, rows), CV_64F);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                double read = 0.0f;
                inStream >> read;
                cameraMatrix.at<double>(r, c) = read;
//                cout << cameraMatrix.at<double>(r, c) << "\n";
            }
        }
        //Distance Coefficients
        inStream >> rows;
        inStream >> columns;

        distanceCoefficients = Mat::zeros(rows, columns, CV_64F);

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                double read = 0.0f;
                inStream >> read;
                distanceCoefficients.at<double>(r, c) = read;
                cout << distanceCoefficients.at<double>(r, c) << "\n";
            }
        }
        inStream.close();
        return true;
    }
    return false;
}

cv::Mat get_object_points() {
    const double MARKER_MM = 30;
    cv::Mat obj_points(4, 1, CV_32FC3);
    obj_points.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-MARKER_MM / 2.f, MARKER_MM / 2.f, 0);
    obj_points.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(MARKER_MM / 2.f, MARKER_MM / 2.f, 0);
    obj_points.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(MARKER_MM / 2.f, -MARKER_MM / 2.f, 0);
    obj_points.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-MARKER_MM / 2.f, -MARKER_MM / 2.f, 0);
    return obj_points;
}

int startWebcamMonitoring(const Mat &cameraMatrix, const Mat &distanceCoefficients, float arucoSquareDimensions) {
    Mat frame;

    list<pair<std::chrono::steady_clock::time_point, vector<Point2f>>> detectedPositions;
    auto obj_points = get_object_points();
    vector<Vec2f> goals;


    int counter = 0;
    Mat goal_image = imread("obstacle.png");
    Vec2f p1_car_position(200, 200);
    Mat p1_car_image = imread("car.png");

    Vec2f p2_car_position(200, 200);
    Mat p2_car_image = imread("car.png");

    Mat detected;

    if ((counter++ % 20 == 0) && (goals.size() < 10)) {
        std::uniform_int_distribution<int> distr_x(0 + goal_image.cols, detected.cols - goal_image.cols);
        std::uniform_int_distribution<int> distr_y(0 + goal_image.rows, detected.rows - goal_image.rows);
        Vec2f p;
        p.val[0] = distr_x(generator);
        p.val[1] = distr_y(generator);
        goals.push_back(p);
    }



    vector<int> markerIds;
    vector<vector<Point2f>> markerCorners, rejectedCandidates;
    aruco::DetectorParameters parameters;

    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(
            aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_250);

    VideoCapture vid(0);

    if (!vid.isOpened()) {
        return -1;
    }

    namedWindow("Webcam", WINDOW_AUTOSIZE);
    vector<Vec3d> rotationVectors, translationVectors;

    while (true) {
        if (!vid.read(frame))
            break;
        aruco::detectMarkers(frame, markerDictionary, markerCorners, markerIds);
        aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimension, cameraMatrix, distanceCoefficients,
                                         rotationVectors, translationVectors);
        aruco::drawDetectedMarkers(frame, markerCorners, markerIds);

        for (int i = 0; i < markerIds.size(); i++) {
            drawFrameAxes(frame, cameraMatrix, distanceCoefficients, rotationVectors[i], translationVectors[i], 0.1f);

            cout << translationVectors[0].val[2] * 60 << " cm \n";

        }
        imshow("Webcam", frame);
        if (waitKey(30) >= 0) {
            break;
        }
    }
    return 1;
}

int main() {

    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
    Mat distanceCoefficients;

    loadCameraCalibration("CameraCalibration.txt", cameraMatrix, distanceCoefficients);
    startWebcamMonitoring(cameraMatrix, distanceCoefficients, 0.099f);
    return 0;
}
