#ifndef DETECT_OBJECTS_HPP
#define DETECT_OBJECTS_HPP

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>

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
void filterLargest(std::vector<Circle>* key_circ, std::vector<Circle>* circ, int max_obj_count, cv::Mat* frame, cv::Scalar color);
int shiftHue(int hue);
void detectObjects(cv::Mat* frame, std::vector<cv::Point2f>* centers, int max_obj_count);

#endif
