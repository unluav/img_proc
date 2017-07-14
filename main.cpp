#include "nav/nav.hpp"
#include "utilities/FrameRateMonitor.h"
#include <cstdio>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

#define VIDEO_PATH "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)I420, framerate=(fraction)30/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	VideoCapture cap(VIDEO_PATH);
	if (!cap.isOpened()) {
		printf("ERROR: Unable to open video file: %s\n", VIDEO_PATH);
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

	Heading head;
	int frame_count = 1, object_count = 1, heading_interval = 5;
	vector<Heading> heads;
	FrameRateMonitor frm;
	frm.Start();

	while (cap.read(frame)) {
		frm.MarkFrame();
	
		detectObjects(&frame, &centers, object_count);
		focusClosestObject(&closest, &origin, &centers);

		printf("\n**************** FRAME %d ****************\n", frame_count);
		printf("    Predicted    (%.1f, %.1f) [%.1f%%]\n", pred->point.x, pred->point.y, pred->confidence * 100);

		arc.predictNextFrame(&closest);
		updateHeading(&head, pred, &origin);
		heads.push_back(head);

		printf("    Actual       (%.1f, %.1f)\n", curr->x, curr->y);
		printf("    Prediction   (%.1f, %.1f) [%.1f%%]\n", pred->point.x, pred->point.y, pred->confidence * 100);
		printf("    Heading      [M: %d, A: %d]\n", head.magnitude, head.angle);

		if (frame_count % heading_interval == 0) {
			averageHeadings(&head, &heads);
			sendToCtrl(&head);
			heads.clear();
			printf("    AVG HEADING  [M: %d, A: %d]\n", head.magnitude, head.angle);
		}

		if (argc == 2 && strcmp(argv[1], "-i") == 0) imshow(VIDEO_PATH, frame);
		if (waitKey(30) >= 0) break;
		frame_count++;
	}

	printf("\n");
	frm.Stop();
	frm.DumpInfo();

	return 0;
}
