#include "nav/nav.hpp"
#include <cstdio>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

#define VIDEO_PATH "/uav_rsc/2roomba.mp4"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	VideoCapture cap;
	if (argc > 1 && strcmp(argv[1], "-l") == 0) {
		cap = VideoCapture("nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)I420, framerate=(fraction)30/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");
	} else if (argc > 1 && strcmp(argv[1], "-i") != 0) {
		cap = VideoCapture(argv[1]);
	} else {
		cap = VideoCapture(VIDEO_PATH);
	}

	if (!cap.isOpened()) {
		return 1;
	}

	Mat frame;
	cap.read(frame);
	Point2f origin(frame.size().width / 2, frame.size().height / 2), closest = origin;
	vector<Point2f> centers(1);

	ConfidenceArc arc;
	arc.setBacktrace(2);
	Point2f* curr = arc.getCurrent();
	Prediction* pred = arc.getPrediction();
	pred->setRange(0.25);

	Heading head;
	int frame_count = 1, object_count = 1, heading_interval = 5;
	vector<Heading> heads;

	while (cap.read(frame)) {
		detectObjects(&frame, &centers, object_count);
		focusClosestObject(&closest, &origin, &centers);
		arc.predictNextFrame(&closest);
		updateHeading(&head, pred, &origin);
		heads.push_back(head);

		if (frame_count % heading_interval == 0) {
			averageHeadings(&head, &heads);
			sendToCtrl(&head);
			heads.clear();
		}

		frame_count++;
	}

	return 0;
}