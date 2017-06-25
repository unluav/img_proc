#include "track_centers.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void countedBubbleSort(vector<Circle>* circles, int n) {
	int size = circles->size();

	for (int i = 1; i <= n; i++) {
		for (int j = 0; j < size - i; j++) {
			if ((*circles)[j].radius > (*circles)[j + 1].radius) {
				Circle temp = (*circles)[j];
				(*circles)[j] = (*circles)[j + 1];
				(*circles)[j + 1] = temp;
			}
		}
	}
}

bool BY_RADIUS(Circle first, Circle second) {
	return first.radius <= second.radius;
}

void findBoundingCircles(vector<vector<Point>>* contours, vector<Circle>* circles, bool draw_circles, Mat* frame, Scalar color) { 
	int size = contours->size(), line_thickness = 1, circle_thickness = 3, line_type = 8;
	float min_radius = 20; // in pixels
	vector<vector<Point>> polygons(size);
	Circle c;

	for (int i = 0; i < size; i++) {
		approxPolyDP(Mat((*contours)[i]), polygons[i], 3, true);
		minEnclosingCircle((Mat) polygons[i], c.center, c.radius);

		if (c.radius > min_radius) {
			circles->push_back(c);
		}
	}

	if (draw_circles) {
		size = circles->size();

		for (int i = 0; i < size; i++) {
			drawContours(*frame, polygons, i, color, line_thickness, line_type, vector<Vec4i>(), 0, Point());
			circle(*frame, (*circles)[i].center, (*circles)[i].radius, color, circle_thickness, line_type, 0);
		}
	}
}

void findLargestCircles(vector<Circle>* key_circles, vector<Circle>* circles, int n, bool draw_circles, Scalar color, Mat* frame) {
	int size = circles->size(), drawn_circles = min(size, n), circle_thickness = 3, line_type = 8;

	if (size > pow(2, drawn_circles)) {
		countedBubbleSort(circles, drawn_circles);
	} else {
		sort(circles->begin(), circles->end(), BY_RADIUS);
	}

	for (int i = 1; i <= drawn_circles; i++) {
		key_circles->push_back((*circles)[size - i]);

		if (draw_circles) {
			circle(*frame, (*circles)[size - i].center, (*circles)[size - i].radius, color, circle_thickness, line_type, 0);
		}
	}
}

void trackCenters(Mat* frame, vector<Point2f>* centers, int object_count) {
	Mat hsv_frame, blobs, red_blobs, lower_red_blobs, upper_red_blobs, green_blobs;
	cvtColor(*frame, hsv_frame, COLOR_BGR2HSV);
	hsv_frame.copyTo(blobs);

	inRange(blobs, Scalar(0, 150, 150), Scalar(25, 250, 250), lower_red_blobs);
	inRange(blobs, Scalar(155, 70, 70), Scalar(180, 200, 200), upper_red_blobs);
	inRange(blobs, Scalar(50, 80, 80), Scalar(130, 255, 255), green_blobs);
	bitwise_or(lower_red_blobs, upper_red_blobs, red_blobs);

	vector<vector<Point>> red_contours, green_contours;
	vector<Vec4i> red_hierarchy, green_hierarchy;
	findContours(red_blobs, red_contours, red_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(green_blobs, green_contours, green_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	int red_count = red_contours.size(), green_count = green_contours.size();
	vector<Circle> red_circles, green_circles, key_circles;
	Scalar red(0, 0, 255), green(0, 255, 0);

	bool draw_circles = false;
	findBoundingCircles(&red_contours, &red_circles, draw_circles, frame, red);
	findBoundingCircles(&green_contours, &green_circles, draw_circles, frame, green);

	draw_circles = true;
	findLargestCircles(&key_circles, &red_circles, object_count / 2, draw_circles, red, frame);
	findLargestCircles(&key_circles, &green_circles, object_count / 2, draw_circles, green, frame);

	centers->clear();

	for (int i = 0; i < key_circles.size(); i++) {
		centers->push_back(key_circles[i].center);
	}
}
