#include <ctime>
#include <iostream>
#include <raspicam/raspicam_cv.h>

using namespace std;

int main (int argc, char **argv) {
	time_t timer_begin, timer_end;
	raspicam::RaspiCam_Cv Camera;
	cv::Mat image;
	int nCount=100;

	Camera.set(cv::CAP_PROP_FORMAT, CV_8UC1);

	cout << "Opening camera..." << endl;
	if (!Camera.open()) {
		cerr << "Error opening the camera" << endl;
		return -1;
	}

	cout << "Capturing " << nCount << " frames ...." << endl;
	time (&timer_begin);
	for (int i=0; i< nCount; i++) {
		Camera.grab();
		Camera.retrieve(image);
		if (i%5==0) cout << "\r captured " << i << " images" 
			<< std::flush;
	}

	cout << "Stop camera..." << endl;
	Camera.release();

	time (&timer_end);
	double secondsElasped = difftime(timer_end, timer_begin);
	cout << secondsElasped << " seconds for " << nCount << 
		" frames : FPS = " << (float)((float)(nCount)/secondsElasped)
		<< endl;
	cv::imwrite("raspicam_cv_image.jpg", image);
	cout << "Image saved at raspicam_cv_image.jpg" << endl;
}
