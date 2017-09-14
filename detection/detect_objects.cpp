#include "detect_objects.hpp"
#include <cstdio>

using namespace std;
using namespace cv;

bool BY_RADIUS(Circle first, Circle second) {
	return first.radius <= second.radius;
}

void findBoundingCircles(vector<vector<Point>>* contours, vector<Circle>* circ) { 
	int size = contours->size(), min_radius = 20;
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

void filterLargest(vector<Circle>* key_circ, vector<Circle>* circ, int max_obj_count, Mat* frame, Scalar color) {
	int size = circ->size(), circ_count = min(size, max_obj_count), j;
	sort(circ->begin(), circ->end(), BY_RADIUS);

	for (int i = 1; i <= circ_count; i++) {
		j = size - i;
		key_circ->push_back((*circ)[j]);
		circle(*frame, (*circ)[j].center, (*circ)[j].radius, color, 3);
		circle(*frame, (*circ)[j].center, 2, color, 3);
	}
}

void detectObjects(Mat* frame, vector<Point2f>* centers, int max_obj_count = 5) {
	cuda::GpuMat d_frame, d_red_blobs, d_grn_blobs;
	Mat h_red_blobs, h_grn_blobs;
	vector<vector<Point>> red_contours, grn_contours;
	vector<Circle> red_circ, grn_circ, key_circ;

	d_frame.upload(*frame);
	cuda::cvtColor(d_frame, d_frame, COLOR_RGB2HSV);

	d_red_blobs = cuda::GpuMat(d_frame.rows, d_frame.cols, CV_8UC1);
	d_grn_blobs = cuda::GpuMat(d_frame.rows, d_frame.cols, CV_8UC1);

	cudaInRange(d_frame, Scalar(120, 120, 110), Scalar(149, 200, 180), d_red_blobs);

	cudaInRange(d_frame, Scalar(35, 70, 20), Scalar(80, 100, 90), d_grn_blobs);

	d_red_blobs.download(h_red_blobs);
	d_grn_blobs.download(h_grn_blobs);

	findContours(h_red_blobs, red_contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	findContours(h_grn_blobs, grn_contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	findBoundingCircles(&red_contours, &red_circ);
	findBoundingCircles(&grn_contours, &grn_circ);

	filterLargest(&key_circ, &red_circ, max_obj_count, frame, Scalar(0, 0, 255));
	filterLargest(&key_circ, &grn_circ, max_obj_count, frame, Scalar(0, 255, 0));

	centers->clear();
	for (int i = 0; i < key_circ.size(); i++) {
		centers->push_back(key_circ[i].center);
	}
}
