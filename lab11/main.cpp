#include <chrono>
#include <list>
#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <random>
#include <tuple>

std::random_device rd;
std::mt19937 generator(rd());

std::pair<cv::Mat, cv::Mat> get_camera_properties_ps3eye() {
    // coefficients for approximated PS3 Eye camera
    //! Camera matrix
    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << 600, 0, 320,
            0, 600, 240,
            0, 0, 1);
    //! Distortion coefficients
    cv::Mat dist_coeffs = (cv::Mat_<double>(5, 1) << 0, 0, 0, 0, 0);

    return {camera_matrix, dist_coeffs};
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

int main() {
    using namespace cv;
    using namespace std;

    list<pair<std::chrono::steady_clock::time_point, vector<Point2f>>> detectedPositions;

    VideoCapture cam(0);
    Ptr<aruco::DetectorParameters> parameters = aruco::DetectorParameters::create();
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_250);
    namedWindow("markers", WINDOW_NORMAL);

    auto [camera_matrix, dist_coeffs] = get_camera_properties_ps3eye();
    auto obj_points = get_object_points();

    Mat goal_image = imread("obstacle.png");
    Vec2f p1_car_position(200, 200);
    Mat p1_car_image = imread("car.png");

    Vec2f p2_car_position(200, 200);
    Mat p2_car_image = imread("car.png");

    vector<Vec2f> goals;
    int counter = 0;
    int p1_points = 0;
    int p2_points = 0;
    while (waitKey(10) != 27) {
        Mat inputImage;
        Mat detected;
        cam >> inputImage;
        // resize(inputImage, inputImage, Size(320, 240), 0, 0, INTER_CUBIC);
        detected = inputImage.clone();

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
        aruco::detectMarkers(inputImage, dictionary, markerCorners,
                             markerIds, parameters, rejectedCandidates);
        Mat outputImage = inputImage.clone();
        aruco::drawDetectedMarkers(detected, markerCorners, markerIds);

        auto found1 = find(markerIds.begin(), markerIds.end(), 13);
        if (found1 != markerIds.end()) {
            int foundIdx = distance(markerIds.begin(), found1);
            auto found = markerCorners.at(foundIdx);

            cv::Vec3d rvecs, tvecs;
            cv::solvePnP(obj_points, found, camera_matrix, dist_coeffs, rvecs, tvecs);
            cv::drawFrameAxes(detected, camera_matrix, dist_coeffs, rvecs, tvecs, 10.1);

            Mat rot_mat;
            Rodrigues(rvecs, rot_mat);
            double angle_z = atan2(rot_mat.at<double>(1, 0), rot_mat.at<double>(0, 0));
            double angle_x = atan2(rot_mat.at<double>(2, 1), rot_mat.at<double>(2, 2));
            angle_x = -angle_x + ((angle_x < 0) ? (-M_PI) : M_PI);
            double angle_y = -asin(rot_mat.at<double>(2, 0));

            // std::cout << angle_x << " " << angle_y << " " << angle_z << std::endl;
            auto new_pos = p1_car_position;
            new_pos.val[0] += 10.0 * angle_y;
            new_pos.val[1] -= 10.0 * angle_x;
            auto new_pos_scr = new_pos;
            new_pos_scr.val[0] -= p1_car_image.cols / 2;
            new_pos_scr.val[1] -= p1_car_image.rows / 2;
            if ((new_pos_scr.val[0] > 0) && (new_pos_scr.val[0] < (detected.cols - p1_car_image.cols - 1)) &&
                (new_pos_scr.val[1] > 0) && (new_pos_scr.val[1] < (detected.rows - p1_car_image.rows - 1))) {
                p1_car_position = new_pos;
            }
        }

        auto found2 = find(markerIds.begin(), markerIds.end(), 23);
        if (found2 != markerIds.end()) {
            int foundIdx = distance(markerIds.begin(), found2);
            auto found = markerCorners.at(foundIdx);

            cv::Vec3d rvecs, tvecs;
            cv::solvePnP(obj_points, found, camera_matrix, dist_coeffs, rvecs, tvecs);
            cv::drawFrameAxes(detected, camera_matrix, dist_coeffs, rvecs, tvecs, 10.1);

            Mat rot_mat;
            Rodrigues(rvecs, rot_mat);
            double angle_z = atan2(rot_mat.at<double>(1, 0), rot_mat.at<double>(0, 0));
            double angle_x = atan2(rot_mat.at<double>(2, 1), rot_mat.at<double>(2, 2));
            angle_x = -angle_x + ((angle_x < 0) ? (-M_PI) : M_PI);
            double angle_y = -asin(rot_mat.at<double>(2, 0));

            // std::cout << angle_x << " " << angle_y << " " << angle_z << std::endl;
            auto new_pos = p2_car_position;
            new_pos.val[0] += 10.0 * angle_y;
            new_pos.val[1] -= 10.0 * angle_x;
            auto new_pos_scr = new_pos;
            new_pos_scr.val[0] -= p2_car_image.cols / 2;
            new_pos_scr.val[1] -= p2_car_image.rows / 2;
            if ((new_pos_scr.val[0] > 0) && (new_pos_scr.val[0] < (detected.cols - p2_car_image.cols - 1)) &&
                (new_pos_scr.val[1] > 0) && (new_pos_scr.val[1] < (detected.rows - p2_car_image.rows - 1))) {
                p2_car_position = new_pos;
            }
        }

        for (auto goal: goals) {
            Mat insetImage(detected,
                           Rect(
                                   goal.val[0] - (goal_image.cols / 2),
                                   goal.val[1] - (goal_image.rows / 2),
                                   goal_image.rows, goal_image.cols));
            goal_image.copyTo(insetImage);
        }

        Mat insetP1Image(detected,
                         Rect(
                                 p1_car_position.val[0] - (p1_car_image.cols / 2),
                                 p1_car_position.val[1] - (p1_car_image.rows / 2),
                                 p1_car_image.rows, p1_car_image.cols));
        p1_car_image.copyTo(insetP1Image);

        Mat insetP2Image(detected,
                         Rect(
                                 p2_car_position.val[0] - (p2_car_image.cols / 2),
                                 p2_car_position.val[1] - (p2_car_image.rows / 2),
                                 p2_car_image.rows, p2_car_image.cols));
        p2_car_image.copyTo(insetP2Image);

        for (int i = 0; i < goals.size(); i++) {
            if (cv::norm(goals[i] - p1_car_position) < (goal_image.cols / 2)) {
                p1_points++;
                std::cout << "p1: " << p1_points << std::endl;
                goals.erase(goals.begin() + i);
                i--;
            }

            if (cv::norm(goals[i] - p2_car_position) < (goal_image.cols / 2)) {
                p2_points++;
                std::cout << "p2: " << p2_points << std::endl;
                goals.erase(goals.begin() + i);
                i--;
            }
        }

        imshow("markers", detected);
    }

    return 0;
}
