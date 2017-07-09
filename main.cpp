#include "nav/nav.hpp"
#include "utilities/FrameRateMonitor.h"
#include <cstdio>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

#define VIDEO_PATH "/uav_rsc/2roomba.mp4"

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
	Point2f closest, origin(frame.size().width / 2, frame.size().height / 2);
	vector<Point2f> centers;

	ConfidenceArc arc;
	arc.setBacktrace(2);
	Point2f* curr = arc.getCurrent();
	Prediction* pred = arc.getPrediction();

	Heading head;
	int frame_count = 1, object_count = 1;
	FrameRateMonitor frm;
	frm.Start();

	while (cap.read(frame)) {
		frm.MarkFrame();
		trackCenters(&frame, &centers, object_count);
		focusClosestObject(&closest, &origin, &centers);

		printf("\n**************** FRAME %d ****************\n", frame_count++);
		printf("    Predicted    (%.1f, %.1f) [%.1f%%]\n", pred->point.x, pred->point.y, pred->confidence * 100);

		arc.predictNextFrame(&closest);
		updateHeading(&head, &arc);
		sendToCtrl(&head);

		printf("    Actual       (%.1f, %.1f)\n", curr->x, curr->y);
		printf("    Prediction   (%.1f, %.1f) [%.1f%%]\n", pred->point.x, pred->point.y, pred->confidence * 100);
		printf("    Heading      [M: %d, A: %d]\n", head.magnitude, head.angle);

		if (argc == 2 && strcmp(argv[1], "-i") == 0) imshow(VIDEO_PATH, frame);
		if (waitKey(30) >= 0) break;
	}

	printf("\n");
	frm.Stop();
	frm.DumpInfo();

	return 0;
}
