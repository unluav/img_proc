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
	Mat cpu_frame;
	gpu::GpuMat gpu_frame, hsv_frame, blobs, red_blobs, lower_red_blobs, upper_red_blobs, green_blobs;
	gpu_frame.upload(*frame);

	gpu::cvtColor(gpu_frame, hsv_frame, COLOR_BGR2HSV);
	hsv_frame.copyTo(blobs);
	gpu_frame.copyTo(lower_red_blobs);
	gpu_frame.copyTo(upper_red_blobs);
	gpu_frame.copyTo(green_blobs);

	Scalar red1(160, 150, 150), red2(180, 255, 255), red3(0, 150, 150), red4(20, 255, 255);
	Scalar green1(50, 80, 80), green2(90, 255, 255);
	
	// inRange_gpu... it's the gift that keeps on giving
	inRange_gpu(blobs, red1, red2, lower_red_blobs);
	inRange_gpu(blobs, red3, red4, upper_red_blobs);
	inRange_gpu(blobs, green1, green2, green_blobs);
	gpu::bitwise_or(lower_red_blobs, upper_red_blobs, red_blobs);

	green_blobs.download(cpu_frame);
	imshow(VID_PATH, cpu_frame);

	vector<vector<Point>> red_contours, green_contours;
	vector<Vec4i> red_hierarchy, green_hierarchy;
	vector<Circle> red_circ, green_circ, key_circ;
	Scalar red(0, 0, 255), green(0, 255, 0);

	Mat cpu_red_blobs, cpu_green_blobs;
	red_blobs.download(cpu_red_blobs);
	green_blobs.download(cpu_green_blobs);
	cvtColor(cpu_red_blobs, cpu_red_blobs, COLOR_BGR2GRAY);
	cvtColor(cpu_green_blobs, cpu_green_blobs, COLOR_BGR2GRAY);

	findContours(cpu_red_blobs, red_contours, red_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(cpu_green_blobs, green_contours, green_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findBoundingCircles(&red_contours, &red_circ, frame, red);
	findBoundingCircles(&green_contours, &green_circ, frame, green);
	filterLargest(&key_circ, &red_circ, object_count, frame, red);
	filterLargest(&key_circ, &green_circ, object_count, frame, green);

	centers->clear();
	for (int i = 0; i < key_circ.size(); i++) {
		centers->push_back(key_circ[i].center);
	}
}
