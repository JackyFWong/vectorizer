#include <iostream>
#include <utility>
#include <cmath>
#include <raspicam/raspicam_cv.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <string>
#include <fstream>

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
    /* FILE FOR OUTPUT */
    unsigned int fileSeq;
    ifstream seqFileIn;
    ofstream seqFileOut;

    seqFileIn.open("sequence.txt", ios::in);

    // if "sequence.txt" exists, read the number & increment
    if (seqFileIn.is_open()) {
        seqFileIn >> fileSeq;
        fileSeq++;
    }
    // otherwise start at 1
    else { fileSeq = 1; }

    ofstream logFile;
    string fileName = "log" + to_string(fileSeq);
    logFile.open(fileName, ios::app);

    #ifdef DEBUG
    cout << "Using file named " << fileName << " for output." << endl;
    #endif

    // increase log file counter by 1
    seqFileOut.open("sequence.txt", ios::out);
    seqFileOut << fileSeq;

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
        cerr << "Error opening the camera" << endl;
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
        
        // print results for max and vector
        #ifdef DEBUG
        cout << "Max: " << maxLoc.x << ", " << maxLoc.y << endl;
        cout << "Vect: mag " << toCenter.first << " deg " << 
            toCenter.second << endl;
        #endif

        // print results to file
        logFile << "Max: " << maxLoc.x << ", " << maxLoc.y << endl;
        logFile << "Vect: mag " << toCenter.first << " deg " << 
            toCenter.second << endl;
    }

    #ifdef DEBUG
    cout << "Stop camera..." << endl;
    #endif

    Camera.release();
    logFile.close();

    return 0;
}
