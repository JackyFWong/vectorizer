#include <iostream>
#include <raspicam/raspicam_cv.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

using namespace std;

int main (int argc, char **argv) {
    raspicam::RaspiCam_Cv Camera;
    cv::Mat image;
    cv::Mat gray;
    int nCount=100;

    // for recording brightest part of img
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;

    Camera.set(cv::CAP_PROP_FORMAT, CV_8UC1);

    cout << "Opening camera..." << endl;
    if (!Camera.open()) {
        cerr << "Error opening the camera" << endl;
        return -1;
    }

    cout << "Capturing " << nCount << " frames ...." << endl;
    for (int i=0; i< nCount; i++) {
        Camera.grab();
        Camera.retrieve(image);

        // convert to grayscale image
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        // find x, y coord of brightest part of img
        cv::minMaxLoc(gray, &minVal, &maxVal, &minLoc, &maxLoc);

        // print results for max
        cout << maxLoc.x << ", " << maxLoc.y << endl;

        // do something with the max value
    }

    cout << "Stop camera..." << endl;
    Camera.release();

    cv::imwrite("raspicam_cv_image.jpg", image);
    cout << "Image saved at raspicam_cv_image.jpg" << endl;
}
