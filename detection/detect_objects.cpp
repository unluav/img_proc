#include "detect_objects.hpp"

using namespace std;
using namespace cv;

bool BY_RADIUS(Circle first, Circle second) {
	return first.radius <= second.radius;
}

void findBoundingCircles(vector<vector<Point>>* contours, vector<Circle>* circ, Mat* frame, Scalar color) { 
	int size = contours->size(), line_thickness = 1, circ_thickness = 3, line_type = 8;
	float min_radius = 20; // in pixels
	vector<vector<Point>> polygons(size);
	Circle c;

	for (int i = 0; i < size; i++) {
		approxPolyDP(Mat((*contours)[i]), polygons[i], 3, true);
		minEnclosingCircle((Mat) polygons[i], c.center, c.radius);

		if (c.radius > min_radius) {
			circ->push_back(c);
		}
	}


CMT	size = circ->size();

CMT	for (int i = 0; i < size; i++) {
CMT		drawContours(*frame, polygons, i, color, line_thickness, line_type, vector<Vec4i>(), 0, Point());
CMT		circle(*frame, (*circ)[i].center, (*circ)[i].radius, color, circ_thickness, line_type, 0);
CMT	}
}

void findLargestCircles(vector<Circle>* key_circ, vector<Circle>* circ, int n, Mat* frame, Scalar color) {
	int size = circ->size(), circ_count = min(size, n), circ_thickness = 3, line_type = 8, index;
	sort(circ->begin(), circ->end(), BY_RADIUS);

	for (int i = 1; i <= circ_count; i++) {
		index = size - i;
		key_circ->push_back((*circ)[index]);

CMT		circle(*frame, (*circ)[index].center, (*circ)[index].radius, color, circ_thickness, line_type, 0);
	}
}

void detectObjects(Mat* frame, vector<Point2f>* centers, int object_count) {
	Mat hsv_frame, blobs, red_blobs, lower_red_blobs, upper_red_blobs, green_blobs;
	GaussianBlur(*frame, *frame, Size(21, 21), 0);
	cvtColor(*frame, hsv_frame, COLOR_BGR2HSV);
	hsv_frame.copyTo(blobs);

	inRange(blobs, Scalar(170, 200, 200), Scalar(180, 255, 255), lower_red_blobs);
	inRange(blobs, Scalar(0, 200, 200), Scalar(10, 255, 255), upper_red_blobs);
	inRange(blobs, Scalar(50, 80, 80), Scalar(90, 255, 255), green_blobs);
	bitwise_or(lower_red_blobs, upper_red_blobs, red_blobs);

	vector<vector<Point>> red_contours, green_contours;
	vector<Vec4i> red_hierarchy, green_hierarchy;
	findContours(red_blobs, red_contours, red_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(green_blobs, green_contours, green_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<Circle> red_circ, green_circ, key_circ;
	Scalar red(0, 0, 255), green(0, 255, 0);

	findBoundingCircles(&red_contours, &red_circ, frame, red);
	findBoundingCircles(&green_contours, &green_circ, frame, green);
	findLargestCircles(&key_circ, &red_circ, object_count, frame, red);
	findLargestCircles(&key_circ, &green_circ, object_count, frame, green);

	centers->clear();
	for (int i = 0; i < key_circ.size(); i++) {
		centers->push_back(key_circ[i].center);
	}
}
