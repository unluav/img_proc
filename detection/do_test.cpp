#include "detect_objects.hpp"
#include <cstdio>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	VideoCapture cap(VIDEO_PATH);
	if (!cap.isOpened()) {
		printf("ERROR: Unable to open video file: %s\n", VIDEO_PATH);
		return 1;
	}

	int frame_count = 1, object_count = 2;
	Mat frame;
	vector<Point2f> centers;

	while (cap.read(frame)) {
		detectObjects(&frame, &centers, object_count);

		cout << frame_count++ << endl;
		for (int i = 0; i < centers.size(); i++) {
			printf("\t(%f, %f)\n", centers[i].x, centers[i].y);
		}

B_CMT
		imshow(VIDEO_PATH, frame);
E_CMT

		if (waitKey(30) >= 0) break;
	}

	return 0;
}
