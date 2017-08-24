#include "detect_objects.hpp"
#include <cstdio>

#define VID_PATH "/uav_rsc/arena-center2017.mp4"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	VideoCapture cap(VID_PATH);
	if (!cap.isOpened()) {
		printf("ERROR: Unable to open video file: %s\n", VID_PATH);
		return 1;
	}

	int frame_count = 1, object_count = 2;
	Mat frame;
	vector<Point2f> centers;

	while (cap.read(frame)) {
		detectObjects(&frame, &centers, object_count);

		printf("\n******** FRAME %d ********\n", frame_count++);
		for (int i = 0; i < centers.size(); i++) {
			printf("\t(%f, %f)\n", centers[i].x, centers[i].y);
		}

		imshow(VID_PATH, frame);
		if (waitKey(1) >= 0) break;
	}

	return 0;
}
