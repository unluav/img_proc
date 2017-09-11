#include "demo.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	VideoCapture cap = VideoCapture("/uav_rsc/lab-roomba.mp4");
	Mat frame, red_blobs, grn_blobs;
	vector<vector<Point>> red_contours, grn_contours;
	FrameRateMonitor frm;

	frm.Start();
	while (cap.read(frame)) {
		// cvtColor(frame, frame, COLOR_RGB2HSV);

		inRange(frame, Scalar(127, 0, 0), Scalar(255, 0, 0), red_blobs);
		inRange(frame, Scalar(0, 127, 0), Scalar(0, 255, 0), grn_blobs);

		findContours(red_blobs, red_contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
		findContours(grn_blobs, grn_contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

		imshow("", frame);
		if (waitKey(1) >= 0) break;
		frm.MarkFrame();
		frm.DumpInfo();
	}

	frm.Stop();
	return 0;
}