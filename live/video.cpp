#include <iostream>
#include <utility>
#include <cmath>
#include <raspicam/raspicam_cv.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>

using namespace std;

// comment out for no debug output
#define DEBUG

// magnitude (pixels), direction (deg from positive x-axis)
typedef pair<double, double> Vect;

// returns distance
double distBtwn(double x1, double x2, double y1, double y2) {
    return sqrt(pow((x2-x1),2) + pow((y2-y1),2));
}

// center x, light x, center y, light y
// returns angle from positive x-axis in degrees
double angle(double x1, double x, double y1, double y) {
    y = y - y1;
    x = x - x1;
    if (x == 0 && y == 0) return 0;
    if (x > 0 && y == 0) return 0;
    if (x == 0 && y > 0) return 90;
    if (x < 0 && y == 0) return 180;
    if (x == 0 && y < 0) return 270;

    // calculation
    double theta = atan(y/x) * (180/3.1415926536);
    if (x < 0 && y > 0)
        return theta + 180;
    else if (x < 0 && y < 0)
        return theta + 180;
    else if (x > 0 && y < 0)
        return theta + 360;
    else
        return theta;
}

int main (int argc, char **argv) {
    /* DATA SETUP FOR CAMERA */
    raspicam::RaspiCam_Cv Camera;
    cv::Mat image;
    int nR, nC;         // numRows, numCols
    cv::Point imgMid;
    Vect toCenter;

    // for recording brightest part of img
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;

    Camera.set(cv::CAP_PROP_FORMAT, CV_8UC1);

    #ifdef DEBUG
    cout << "Opening camera..." << endl;
    if (!Camera.open()) {
        cout << "Error opening the camera" << endl;
        return -1;
    }
    cout << "Capturing and analyzing indefinitely..." << endl;
    #endif

    while(1) {
        Camera.grab();
        Camera.retrieve(image);
        nR = image.rows;
        nC = image.cols;
        imgMid.x = nC / 2;
        imgMid.y = nR / 2;

        // find x, y coord of brightest part of img
        cv::minMaxLoc(image, &minVal, &maxVal, &minLoc, &maxLoc);

        // calculate vector to the center of the camera
        toCenter.first = distBtwn(imgMid.x, maxLoc.x, imgMid.y, maxLoc.y);
        toCenter.second = angle(imgMid.x, maxLoc.x, imgMid.y, maxLoc.y);

        auto timeNow = chrono::system_clock::to_time_t(chrono::system_clock::now());
        
        // print results for max and vector
        #ifdef DEBUG
        cout << ctime(&timeNow);
        cout << "Max: " << maxLoc.x << ", " << maxLoc.y << endl;
        cout << "Vect: mag " << toCenter.first << " deg " << 
            toCenter.second << "\n" << endl;
        #endif

    }

    #ifdef DEBUG
    cout << "Stop camera..." << endl;
    #endif

    Camera.release();

    return 0;
}
