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

int shiftHue(int hue) {
	return (hue + 30) % 180;
}

void detectObjects(Mat* frame, vector<Point2f>* centers, int max_obj_count = 5) {
	cuda::GpuMat d_frame;
	Mat h_frame, h_red_blobs, h_grn_blobs;

	d_frame.upload(*frame);
	cuda::cvtColor(d_frame, d_frame, COLOR_BGR2HSV);
	d_frame.download(h_frame);

	for (int i = 0; i < h_frame.rows; i++) {
        for (int j = 0; j < h_frame.cols; j++) {
			h_frame.at<Vec3b>(i, j)[0] = shiftHue(h_frame.at<Vec3b>(i, j)[0]);
        }
    }

	inRange(h_frame, Scalar(shiftHue(160), 150, 150), Scalar(shiftHue(20), 255, 255), h_red_blobs);
	inRange(h_frame, Scalar(shiftHue(50), 80, 80), Scalar(shiftHue(90), 255, 255), h_grn_blobs);

	vector<vector<Point>> red_contours, grn_contours;
	vector<Circle> red_circ, grn_circ, key_circ;

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
