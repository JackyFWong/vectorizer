#include <iostream>
#include <utility>
#include <cmath>
#include <raspicam/raspicam_cv.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

using namespace std;

// comment out for no debug output
#define DEBUG

// magn(pixels), direction (deg from right)
typedef pair<double, double> Vect;

double distBtwn(double x1, double x2, double y1, double y2) {
    return sqrt(pow((x2-x1),2) + pow((y2-y1),2));
}

// center x, light x, center y, light y
double angle(double x1, double x, double y1, double y) {
    y = y - y1;
    x = x - x1;
    if (x == 0 && y == 0) return 0;
    if (x > 0 && y == 0) return 0;
    if (x == 0 && y > 0) return 90;
    if (x < 0 && y == 0) return 180;
    if (x == 0 && y < 0) return 270;

    // calculation
    double theta = atan(y, x) * (180/3.1415926536);
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
    // data setup
    raspicam::RaspiCam_Cv Camera;
    cv::Mat image;
    cv::Mat gray;
    int nCount=100;
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
        cerr << "Error opening the camera" << endl;
        return -1;
    }
    cout << "Capturing " << nCount << " frames ...." << endl;
    #endif

    for (int i=0; i< nCount; i++) {     // todo: change to infinite loop
        Camera.grab();
        Camera.retrieve(image);
        nR = image.rows;
        nC = image.cols;
        imgMid.x = nC / 2;
        imgMid.y = nR / 2;

        // convert to grayscale image
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        // find x, y coord of brightest part of img
        cv::minMaxLoc(gray, &minVal, &maxVal, &minLoc, &maxLoc);

        // calculate vector to the center of the camera
        toCenter.first = distBtwn(imgMid.x, maxLoc.x, imgMid.y, maxLoc.y);
        toCenter.second = angle(imgMid.x, maxLoc.x, imgMid.y, maxLoc.y);
        
        // print results for max and vector
        #ifdef DEBUG
        cout << "Max: " << maxLoc.x << ", " << maxLoc.y << endl;
        cout << "Vect: mag " << toCenter.first << " deg " << 
            toCenter.second << endl;
        #endif
    }

    #ifdef DEBUG
    cout << "Stop camera..." << endl;
    #endif
    Camera.release();
}
