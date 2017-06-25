#ifndef TRACK_CENTERS_HPP
#define TRACK_CENTERS_HPP

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

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

void countedBubbleSort(vector<Circle>* circles, int n);
bool BY_RADIUS(Circle first, Circle second);
void findBoundingCircles(Mat* frame, vector<vector<Point>>* contours, vector<vector<Point>>* poly, vector<Circle>* circles, Scalar color, bool draw_circles);
void findLargestCircles(vector<Circle>* key_circles, vector<Circle>* circles, int n, bool draw_circles, Scalar color, Mat* frame);
void fetchCenters(Mat* frame, vector<Point2f>* centers, int object_count);

#endif
