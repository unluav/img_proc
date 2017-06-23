// #include "track_centers.hpp"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	if (argc < 1) {
		cout << "USAGE: " << argv[0] << " VIDEO_FILE" << endl;
	}

	VideoCapture cap(argv[1]);
	if (!cap.isOpened()) {
		cout << "ERROR: Unable to open video file" << endl;
		return 1;
	}

	int frame_count = 1;
	Mat frame;

	while (1) {
		cap >> frame;
		imshow(argv[1], frame);
		cout << frame_count++ << endl;
		if (waitKey(30) >= 0) break;
	}

	return 0;
}
