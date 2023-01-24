#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

bool compareContourAreas(vector<Point> contour1, vector<Point> contour2) {
    double i = fabs(contourArea(Mat(contour1)));
    double j = fabs(contourArea(Mat(contour2)));
    return (i < j);
}

int main(int argc, char **argv) {
    vector<int> lower = {0, 141, 87};
    vector<int> upper = {18, 255, 255};
    namedWindow("options", cv::WINDOW_AUTOSIZE);
    createTrackbar("lh", "options", &lower[0], 255);
    createTrackbar("ls", "options", &lower[1], 255);
    createTrackbar("lv", "options", &lower[2], 255);
    createTrackbar("hh", "options", &upper[0], 255);
    createTrackbar("hs", "options", &upper[1], 255);
    createTrackbar("hv", "options", &upper[2], 255);

    VideoCapture camera(0);
    if (!camera.isOpened())
        return -1;

    while (waitKey(1) != 27) {
        Mat raw_img;
        camera.read(raw_img);
        Mat source_img;
        flip(raw_img, source_img, 1);

        Mat hsv_img;
        Mat red_img;

        Mat img_edges;


        cvtColor(source_img, hsv_img, COLOR_BGR2HSV);
        inRange(hsv_img, Scalar(lower[0], lower[1], lower[2]), Scalar(upper[0], upper[1], upper[2]), red_img);

        Mat kern = getStructuringElement(MORPH_ELLIPSE, {50, 50});
        morphologyEx(red_img, red_img, MORPH_CLOSE, kern);


        vector<vector<Point>> contours;

        findContours(red_img, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
//        drawContours(source_img, contours, -1, {255, 255, 255});

        sort(contours.begin(), contours.end(), compareContourAreas);


        if (contours.size() > 1) {
            vector<Point> object1BiggestContour = contours[contours.size() - 1];
            vector<Point> object2BiggestContour = contours[contours.size() - 2];

            auto m = moments(object1BiggestContour, false);
            Point object1point1 = {(int) (m.m10 / m.m00), (int) (m.m01 / m.m00)};
            Point object1point2 = {(int) ((m.m10 / m.m00)), (int) ((m.m01 / m.m00) - 4)};
            Point object1point3 = {(int) ((m.m10 / m.m00)), (int) ((m.m01 / m.m00) - 8)};
            int x = (int) (m.m10 / m.m00);
            int y = (int) (m.m01 / m.m00);

            auto m1 = moments(object2BiggestContour, false);
            Point object2point1 = {(int) (m1.m10 / m1.m00), (int) (m1.m01 / m1.m00)};
            Point object2point2 = {(int) ((m1.m10 / m1.m00)), (int) ((m1.m01 / m1.m00) - 4)};
            Point object2point3 = {(int) ((m1.m10 / m1.m00)), (int) ((m1.m01 / m1.m00) - 8)};
            int x1 = (int) (m1.m10 / m1.m00);
            int y1 = (int) (m1.m01 / m1.m00);

            if (abs(y1 - y) < 20) {
                line(source_img, object1point1, object2point1, cv::Scalar(255, 0, 0), 2, cv::LINE_8);
                line(source_img, object1point2, object2point2, cv::Scalar(0, 255, 0), 2, cv::LINE_8);
                line(source_img, object1point3, object2point3, cv::Scalar(0, 0, 255), 2, cv::LINE_8);
            }
        }

        imshow("Output", source_img);
        imshow("red_image", red_img);
    }
    return 0;
}