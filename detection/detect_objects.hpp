#ifndef DETECT_OBJECTS_HPP
#define DETECT_OBJECTS_HPP

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
// #include <opencv2/gpu.hpp>

#define VID_PATH "/uav_rsc/2roomba.mp4"

struct Circle {
	cv::Point2f center;
	float radius;

	Circle() {
		center = cv::Point2f(0.0f, 0.0f);
		radius = 0.0f;
	}

	Circle(cv::Point2f c, float r) {
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
void findBoundingCircles(std::vector<std::vector<cv::Point>>* contours, std::vector<Circle>* circ);
void filterLargest(std::vector<Circle>* key_circ, std::vector<Circle>* circ, int max_count, cv::Mat* frame, cv::Scalar color);
void detectObjects(cv::Mat* frame, std::vector<cv::Point2f>* centers, int object_count);

#endif
