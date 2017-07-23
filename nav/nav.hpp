#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include "../detection/detect_objects.hpp"
#include "../tracking/conf_arc.hpp" 

struct Heading {
	uint16_t magnitude;
	uint16_t angle;
};

void sendToCtrl(Heading* head);
void focusClosestObject(cv::Point2f* closest, cv::Point2f* origin, std::vector<cv::Point2f>* centers);
void updateHeading(Heading* head, Prediction* pred, cv::Point2f* origin);
void averageHeadings(Heading* head, std::vector<Heading>* heads);
