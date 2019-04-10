#include <ctime>
#include <iostream>
#include <raspicam/raspicam_cv.h>

int main ( int argc, char** argv ) {

    time_t timer_begin, timer_end;
    raspicam::RaspiCam_Cv Camera;
    cv:: Mat image;
    int nCount = 100;

    // camera param
    Camera.set( CV_CAP_PROP_FORMAT, CV_BUG1 );
    // open
    cout << "Opening camera..." << endl;
    if ( !Camera.open() ) {
        cerr << "Error opening camera." << endl;
        return -1;
    }

    // capture
    cout << "Capturing " << nCount << " frames ..." << endl;
    time ( &timer_begin);
    for (int i=0; i < nCount; i++) {
        Camera.grab();
        Camera.retrieve( image );
        if (i % 5 == 0) cout << "\r captured " << i << " images" << std::flush;
    }
    cout << "Stop camera." << endl;
    Camera.release();

    // time stats
    time ( &timer_end);
    double secElapased = difftime( timer_end, timer_begin );
    cout << secElapsed << " seconds for " << nCount << " frames : FPS = " << 
        (float)((float)nCount / secElapased) << endl;

    // save img
    cv::imwrite("raspicam_cv_image.jpg", image);
    cout << "Image saved at raspicam_cv_image.jpg" << endl;

}
