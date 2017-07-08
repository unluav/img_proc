#include "nav/nav.hpp"
#include <iostream>
#include <cstdio>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	if (argc < 2) {
		cout << "USAGE: " << argv[0] << " VIDEO_FILE" << endl;
		return 1;
	}

	VideoCapture cap(argv[1]);
	if (!cap.isOpened()) {
		cout << "ERROR: Unable to open video file" << endl;
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

	Heading hdg;
	int frame_count = 1, object_count = 1;

	while (cap.read(frame)) {
		imshow(argv[1], frame);

		trackCenters(&frame, &centers, object_count);
		focusClosestObject(&closest, &origin, &centers);

		printf("\n\n************************ FRAME %d ************************", frame_count++);
		printf("\n\tPredicted    (%.1f, %.1f) [%.1f%%]", pred->point.x, pred->point.y, pred->confidence * 100);

		arc.predictNextFrame(&closest);
		updateHeading(&hdg, &arc);

		printf("\n\tActual       (%.1f, %.1f)", curr->x, curr->y);
		printf("\n\tPrediction   (%.1f, %.1f) [%.1f%%]", pred->point.x, pred->point.y, pred->confidence * 100);
		printf("\n\tHeading      [M: %d, A: %d]", hdg.magnitude, hdg.angle);

		if (waitKey(30) >= 0) break;
	}

	printf("\n");
	return 0;
}
