#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

#include "../object_detection/track_centers.hpp"
#include "../object_tracking/conf_arc.hpp"

struct Heading {
	uint16_t magnitude;
	uint16_t angle;
}

cv::Point2f focusObject(cv::Point2f* position, std::vector<cv::Point2f>* centers);
Heading suggestHeading(ConfidenceArc* arc);
