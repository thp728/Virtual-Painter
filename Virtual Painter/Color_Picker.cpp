#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

/*
Color Picker to create a mask of a color using webcam
*/

int main() {

	VideoCapture cap(0);
	Mat frame;
	Mat frameHSV, mask, frameColor;

	int hmin = 0, smin = 0, vmin = 0;
	int hmax = 179, smax = 255, vmax = 255;

	// Create new window with trackbars
	namedWindow("Trackbars", (640, 200)); // size of window is 640x200 
	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 255);

	while (true) {
		cap.read(frame);
		// convert to HSV
		cvtColor(frame, frameHSV, COLOR_BGR2HSV);

		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);

		inRange(frameHSV, lower, upper, mask);
		cout << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax << endl;

		// yellow marker: 30,23,179,34,255,255

		imshow("Frame", frame);
		imshow("Selected color", mask);
		waitKey(1);
	}

	return 0;
}