#ifndef DETECT_OBJECTS_HPP
#define DETECT_OBJECTS_HPP

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

#define VIDEO_PATH "/uav_rsc/2roomba.mp4"
#define B_CMT
#define E_CMT

using namespace std;
using namespace cv;

struct Circle {
	Point2f center;
	float radius;

	Circle() {
		center = Point2f(0.0f, 0.0f);
		radius = 0.0f;
	}

	Circle(Point2f c, float r) {
		center = c;
		radius = r;
	}

	Circle(const Circle& other) {
		center = other.center;
		radius = other.radius;
	}

	Circle(Circle* other) {
		center = other->center;
		radius = other->radius;
	}

	Circle& operator=(const Circle& other) {
		if (this != &other) {
			center = other.center;
			radius = other.radius;
		}

		return *this;
	}
};

bool BY_RADIUS(Circle first, Circle second);
void findBoundingCircles(vector<vector<Point>>* contours, vector<Circle>* circ, bool draw_circ, Mat* frame, Scalar color);
void findLargestCircles(vector<Circle>* key_circ, vector<Circle>* circ, int n, bool draw_circ, Mat* frame, Scalar color);
void detectObjects(Mat* frame, vector<Point2f>* centers, int object_count);

#endif
