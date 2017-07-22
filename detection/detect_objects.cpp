#include "detect_objects.hpp"

using namespace std;
using namespace cv;

bool BY_RADIUS(Circle first, Circle second) {
	return first.radius <= second.radius;
}

void findBoundingCircles(vector<vector<Point>>* contours, vector<Circle>* circ, Mat* frame, Scalar color) { 
	int size = contours->size();
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

	// size = circ->size();
	// for (int i = 0; i < size; i++) {
	// 	drawContours(*frame, polygons, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	// 	circle(*frame, (*circ)[i].center, (*circ)[i].radius, color, 3, 8, 0);
	// }
}

void filterLargest(vector<Circle>* key_circ, vector<Circle>* circ, int max_count, Mat* frame, Scalar color) {
	int size = circ->size(), circ_count = min(size, max_count), j;
	sort(circ->begin(), circ->end(), BY_RADIUS);

	for (int i = 1; i <= circ_count; i++) {
		j = size - i;
		key_circ->push_back((*circ)[j]);

		circle(*frame, (*circ)[j].center, (*circ)[j].radius, color, 3, 8, 0);
		circle(*frame, (*circ)[j].center, 1, color, 3, 8, 0);
	}
}

void detectObjects(Mat* frame, vector<Point2f>* centers, int object_count = 5) {
	Mat hsv_frame, blobs, red_blobs, lower_red_blobs, upper_red_blobs, green_blobs;
	GaussianBlur(*frame, *frame, Size(21, 21), 0);
	cvtColor(*frame, hsv_frame, COLOR_BGR2HSV);
	hsv_frame.copyTo(blobs);

	inRange(blobs, Scalar(160, 150, 150), Scalar(180, 255, 255), lower_red_blobs);
	inRange(blobs, Scalar(0, 150, 150), Scalar(20, 255, 255), upper_red_blobs);
	inRange(blobs, Scalar(50, 80, 80), Scalar(90, 255, 255), green_blobs);
	bitwise_or(lower_red_blobs, upper_red_blobs, red_blobs);

	vector<vector<Point>> red_contours, green_contours;
	vector<Vec4i> red_hierarchy, green_hierarchy;
	vector<Circle> red_circ, green_circ, key_circ;
	Scalar red(0, 0, 255), green(0, 255, 0);

	findContours(red_blobs, red_contours, red_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(green_blobs, green_contours, green_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findBoundingCircles(&red_contours, &red_circ, frame, red);
	findBoundingCircles(&green_contours, &green_circ, frame, green);
	filterLargest(&key_circ, &red_circ, object_count, frame, red);
	filterLargest(&key_circ, &green_circ, object_count, frame, green);

	centers->clear();
	for (int i = 0; i < key_circ.size(); i++) {
		centers->push_back(key_circ[i].center);
	}
}
