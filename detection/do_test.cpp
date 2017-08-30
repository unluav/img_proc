#include "detect_objects.hpp"
#include <cstdio>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	string video_path = argc > 1 ? argv[1] : "/uav_rsc/arena-center2017.mp4";
	VideoCapture cap(video_path);
	int frame_count = 1;
	Mat frame;
	vector<Point2f> centers;

	if (!cap.isOpened()) {
		printf("ERROR: Unable to open video file %s\n", video_path.c_str());
		return 1;
	}

	while (cap.read(frame)) {
		detectObjects(&frame, &centers, 2);

		printf("**************** FRAME %d ****************\n", frame_count++);
		for (int i = 0; i < centers.size(); i++) {
			printf("\t(%.1f, %.1f)", centers[i].x, centers[i].y);
		}
		printf("\n\n");

		imshow(video_path, frame);
		if (waitKey(1) >= 0) break;
	}

	return 0;
}
