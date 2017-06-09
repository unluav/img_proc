#include "object_tracking/conf_arc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Point2f* focusObject(Point2f* origin, Point2f centers[], int size) {
	int closest = 0;
	Point2f diff(0, 0);
	double min_dist = norm(diff);

	for (int i = 0; i < size; i++) {
		diff = Point2f(centers[i].x - origin->x, centers[i].y - origin->y);
		double dist = norm(diff);

		if (dist < min_dist) {
			min_dist = dist;
			closest = i;
		}
	}

	return &centers[closest];
}

int main(int argc, char** argv) {
	Point2f centers[25];
	ConfidenceArc arc;
	Prediction* pred = arc.getPrediction();
	int size = 0;

	CvCapture* feed(0);
	IplImage* frame = cvQueryFrame(feed);
	Point2f origin((double) frame->width / 2, (double) frame->height / 2);

	while (frame != NULL) {
		size = CenterTracking(centers, frame);
		Point2f* focused = focusObject(&origin, centers, size);
		arc.predictNextFrame(focused);

		// TODO: echo proper movement commands to embedded system
		Point2f move(pred->point.x - origin.x, pred->point.y - origin.y);
		double conf = pred->confidence;

		frame = cvQueryFrame(feed);
	}

	return 0;
}
