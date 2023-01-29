#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <chrono>
#include <list>

using namespace std;
using namespace cv;

void createArucoMarkers() {
    Mat outputMarker;
    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(
            aruco::PREDEFINED_DICTIONARY_NAME::DICT_6X6_250);

    for (int i = 0; i < 50; i++) {
        aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
        ostringstream convert;
        string imageName = "4x4Marker_";
        convert << imageName << i << ".jpg";
        imwrite(convert.str(), outputMarker);
    }
}

int main() {
    createArucoMarkers();
    return 0;
}
