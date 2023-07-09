#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

/* Paint on the screen using a predefined color set using Color_Picker.cpp */

VideoCapture cap(0);
Mat frame;
vector<vector<int>> newPoints;

// Color values set using Color_Picker.cpp for mask
vector<vector<int>> colors { {30, 23, 179, 34, 255, 255} }; // Yellow Marker

// Colors which will actually be painted on the canvas
vector<Scalar> colorValues { {0,0,255} }; // BGR for Red

// Returns the point where to start painting on the canvas
Point getContours(Mat img) {

	Point myPt(0, 0);

	vector<vector<Point>> contours; // will contain corner points of each contour
	vector<Vec4i> hierarchy;

	findContours(img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size()); // will contain approximated polygon corner points
	vector<Rect> boundRect(contours.size()); // will contain corners of bounding boxes for conPoly

	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);

		// filter out smaller contours
		if (area > 1000) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			boundRect[i] = boundingRect(conPoly[i]);

			myPt.x = boundRect[i].x + (boundRect[i].width / 2);
			myPt.y = boundRect[i].y;
		}
	}

	return myPt;
}

// Returns array of points and their corresponding color to be painted
vector<vector<int>> findColor(Mat img) {

	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < colors.size(); i++) {
		Scalar lower(colors[i][0], colors[i][1], colors[i][2]);
		Scalar upper(colors[i][3], colors[i][4], colors[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);

		Point myPt = getContours(mask);
		if (myPt.x != 0) {
			newPoints.push_back( {myPt.x, myPt.y, i} );
		}
	}

	return newPoints;
}

void drawOnCanvas(vector<vector<int>> points, vector<Scalar> colorValues) {
	
	for (int i = 0; i < points.size(); i++) {
		circle(frame, Point(points[i][0], points[i][1]), 10, colorValues[points[i][2]], FILLED);
	}

}

int main() {

	while (true) {
		cap.read(frame);
		if (frame.empty())
			break;

		newPoints = findColor(frame);
		drawOnCanvas(newPoints, colorValues);

		imshow("Canvas", frame);
		waitKey(1);
	}

	return 0;
}