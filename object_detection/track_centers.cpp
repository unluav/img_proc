#include "track_centers.hpp"
#include <iostream>
 
using namespace std;
using namespace cv;

vector<Circle> countedBubbleSort(vector<Circle> circles, int n) {
	int size = circles.size();

	for (int i = 1; i <= n; i++) {
		for (int j = 0; j < size - i; j++) {
			if (circles[j].radius > circles[j + 1].radius) {
				Circle temp = circles[j];
				circles[j] = circles[j + 1];
				circles[j + 1] = temp;
			}
		}
	}

	return circles;
}

bool BY_RADIUS(Circle first, Circle second) {
	return first.radius <= second.radius;
}

void findBoundingCircles(Mat* frame, vector<vector<Point>>* contours, vector<vector<Point>>* poly, vector<Circle> circles, Scalar color, bool draw_circles) {
	int line_thickness = 1, circle_thickness = 3, line_type = 8;

	for (int i = 0; i < contours->size(); i++) {
		approxPolyDP(Mat((*contours)[i]), (*poly)[i], 3, true);
		minEnclosingCircle((Mat) (*poly)[i], circles[i].center, circles[i].radius);
	}

	if (draw_circles) {
		int planned_circles = 1, size = circles.size(), drawn_circles = min(size, planned_circles);
		circles = countedBubbleSort(circles, planned_circles);
//		sort(circles.begin, circles.end, BY_RADIUS);

		for (int i = 1; i <= drawn_circles; i++) {
//			drawContours(*frame, *poly, size - i, color, line_thickness, line_type, vector<Vec4i>(), 0, Point());
			circle(*frame, circles[size - i].center, circles[size - i].radius, color, circle_thickness, line_type, 0);
		}
	}
}

void fetchCenters(Mat* frame) {
	Mat hsv_frame, temp_blobs, red_blobs, lower_red_blobs, upper_red_blobs, green_blobs;
	cvtColor(*frame, hsv_frame, COLOR_BGR2HSV);
	hsv_frame.copyTo(temp_blobs);

	inRange(temp_blobs, Scalar(0, 150, 150), Scalar(25, 250, 250), lower_red_blobs);
	inRange(temp_blobs, Scalar(155, 70, 70), Scalar(180, 200, 200), upper_red_blobs);
	inRange(temp_blobs, Scalar(50, 80, 80), Scalar(130, 255, 255), green_blobs);

	bitwise_or(lower_red_blobs, upper_red_blobs, red_blobs);

	vector<vector<Point>> red_contours, green_contours;
	vector<Vec4i> red_hierarchy, green_hierarchy;
	findContours(red_blobs, red_contours, red_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(green_blobs, green_contours, green_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	int red_count = red_contours.size(), green_count = green_contours.size();
	vector<vector<Point>> red_poly(red_count), green_poly(green_count);
	vector<Circle> red_circles(red_count), green_circles(green_count);
	Scalar red(0, 0, 255), green(0, 255, 0);
	bool draw_circles = true;

	findBoundingCircles(frame, &red_contours, &red_poly, red_circles, red, draw_circles);
	findBoundingCircles(frame, &green_contours, &green_poly, green_circles, green, draw_circles);
}

/*int fetchCenters(Point2f centers[], IplImage *frame) {
	IplImage *blobs = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	Mat temp_blobs, red_blobs, lower_red_blobs, upper_red_blobs, green_blobs;
	cvarrToMat(blobs).copyTo(temp_blobs);	

	// Finding the color blobs (needs to be a colored image) using Inrange
	inRange(temp_blobs, Scalar(0, 150, 150), Scalar(25, 250, 250), lower_red_blobs);
	inRange(temp_blobs, Scalar(155, 70, 70), Scalar(180, 200, 200), upper_red_blobs);
	inRange(temp_blobs, Scalar(50, 80, 80), Scalar(130, 255, 255), green_blobs);

	bitwise_or(lower_red_blobs, upper_red_blobs, red_blobs);

	vector<vector<Point>> red_contours, green_contours;
	vector<Vec4i> red_hierarchy, green_hierarchy;
	findContours(red_blobs, red_contours, red_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(green_blobs, green_contours, green_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// calculate min enclosing circles for every red and green contour
	int red_count = red_contours.size(), green_count = green_contours.size();
	vector<vector<Point>> red_poly(red_count), green_poly(green_count);
	vector<Circle> red_circles(red_count), green_circles(green_count);

	for (int i = 0; i < red_count; i++) {
		approxPolyDP(Mat(red_contours[i]), red_poly[i], 3, true);
		minEnclosingCircle((Mat) red_poly[i], red_circles[i].center, red_circles[i].radius);
	}

	for (int i = 0; i < green_count; i++) {
		approxPolyDP(Mat(green_contours[i]), green_poly[i], 3, true);
		minEnclosingCircle((Mat) green_poly[i], green_circles[i].center, green_circles[i].radius);
	}

	red_count = 5, green_count = 5;
	vector<Circle> key_red_circles(red_count), key_green_circles(green_count);

	sort(red_circles.begin(), red_circles.end(), BY_RADIUS);
	sort(green_circles.begin(), green_circles.end(), BY_RADIUS);

	findLargest(&red_count, &red_circles, &key_red_circles);
	findLargest(&green_count, &green_circles, &key_green_circles);
	int total_count = red_count + green_count;

	cout << "\tKEY RED CIRCLES" << endl << "\t";
	for (int i = 0; i < total_count; i++) {
		if (key_red_circles[i].radius > 20) {
			cout << "[(" << key_red_circles[i].center.x << ", " << key_red_circles[i].center.y << ")\t";
			cout << key_red_circles[i].radius << "]\t";
		}
	}

	cout << endl;

	cout << "\tKEY GREEN CIRCLES" << endl << "\t";
	for (int i = 0; i < total_count; i++) {
		if (key_green_circles[i].radius > 20) {
			cout << "[(" << key_green_circles[i].center.x << ", " << key_green_circles[i].center.y << ")\t";
			cout << key_green_circles[i].radius << "]\t";
		}
	}

	cout << endl;

	// combine red and green arrays into one array and fill rest of array with -1,-1
	int size = sizeof (Point2f*) / sizeof (Point2f);

	for (int i = 0; i < size; i++) {
		if (i < red_count) {
			centers[i] = key_red_circles[i].center;
		} else if (i < total_count) {
			centers[i] = key_green_circles[i - red_count].center;
		} else {
			centers[i] = Point2f(-1, -1);
		}
	}

	// return number of detected objects
	return total_count;
}*/
