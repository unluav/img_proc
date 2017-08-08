#include "detect_objects.hpp"

using namespace std;
using namespace cv;

bool BY_RADIUS(Circle first, Circle second) {
	return first.radius <= second.radius;
}

void findBoundingCircles(vector<vector<Point>>* contours, vector<Circle>* circ) { 
	int size = contours->size();
	float min_radius = 20;
	vector<vector<Point>> polygons(size);
	Circle c;

	for (int i = 0; i < size; i++) {
		approxPolyDP(Mat((*contours)[i]), polygons[i], 3, true);
		minEnclosingCircle((Mat) polygons[i], c.center, c.radius);

		if (c.radius > min_radius) {
			circ->push_back(c);
		}
	}
}

void filterLargest(vector<Circle>* key_circ, vector<Circle>* circ, int max_count, Mat* frame, Scalar color) {
	int size = circ->size(), circ_count = min(size, max_count), j;
	sort(circ->begin(), circ->end(), BY_RADIUS);

	for (int i = 1; i <= circ_count; i++) {
		j = size - i;
		key_circ->push_back((*circ)[j]);
		circle(*frame, (*circ)[j].center, (*circ)[j].radius, color, 3);
		circle(*frame, (*circ)[j].center, 2, color, 3);
	}
}

void detectObjects(Mat* frame, vector<Point2f>* centers, int obj_count = 5) {
	cuda::GpuMat d_frame;
	Mat h_frame, h_red_blobs, h_lwr_red_blobs, h_upr_red_blobs, h_grn_blobs;
	Scalar lowest_red(0, 150, 150), lower_red(20, 255, 255);
	Scalar upper_red(160, 150, 150), uppest_red(180, 255, 255);
	Scalar lower_grn(50, 80, 80), upper_grn(90, 255, 255);
	Scalar red(0, 0, 255), grn(0, 255, 0);
	
	d_frame.upload(*frame);
	cuda::cvtColor(d_frame, d_frame, COLOR_BGR2HSV);
	d_frame.download(h_frame);
	
	inRange(h_frame, lowest_red, lower_red, h_lwr_red_blobs);
	inRange(h_frame, upper_red, uppest_red, h_upr_red_blobs);
	inRange(h_frame, lower_grn, upper_grn, h_grn_blobs);
	bitwise_or(h_lwr_red_blobs, h_upr_red_blobs, h_red_blobs);

	vector<vector<Point>> red_contours, grn_contours;
	vector<Vec4i> red_hierarchy, grn_hierarchy;
	vector<Circle> red_circ, grn_circ, key_circ;

	findContours(h_red_blobs, red_contours, red_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(h_grn_blobs, grn_contours, grn_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findBoundingCircles(&red_contours, &red_circ);
	findBoundingCircles(&grn_contours, &grn_circ);
	filterLargest(&key_circ, &red_circ, obj_count, frame, red);
	filterLargest(&key_circ, &grn_circ, obj_count, frame, grn);

	centers->clear();
	for (int i = 0; i < key_circ.size(); i++) {
		centers->push_back(key_circ[i].center);
	}
}
