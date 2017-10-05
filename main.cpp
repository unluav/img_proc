#include "nav/nav.hpp"
#include "detection/grid_detect.hpp"
#include "utilities/FrameRateMonitor.h"
#include <cstdio>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int find(char** a, int len, const char* el) {
	int pos = -1;
	for (int i = 0; i < len; i++) {
		if (strcmp(a[i], el) == 0) {
			pos = i;
			break;
		}
	}

	return pos;
}

int main(int argc, char** argv) {
	bool show_images = find(argv, argc, "-i") > 0;
	VideoCapture cap = VideoCapture("/uav_rsc/lab-roomba.mp4");
	
	if (find(argv, argc, "-l") > 0) {
		cap = VideoCapture("nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)I420, framerate=(fraction)30/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");
	} else if (argc > 1 && find(argv, argc, "-i") != 1) {
		cap = VideoCapture(argv[1]);
	}

	Mat frame;
	ConfidenceArc arc;
	vector<Point2f> centers(1);
	Heading heading;
	vector<Heading> heading_record;
	FrameRateMonitor frm;

	int frame_count = 1;
	cap.read(frame);
	Point2f origin(frame.size().width / 2, frame.size().height / 2), closest_obj = origin;
	Point2f* curr = arc.getCurrent();
	Prediction* pred = arc.getPrediction();
	frm.Start();

	// Configurables
	int heading_interval = 5;	// Number of frames over which the heading is averaged
	int max_obj_count = 2;		// Maximum number of objects of each color
	pred->setRange(0.25);		// Decimal value representing the error threshold percentage for predictions
	arc.setBacktrace(2);		// Number of previous frames to learn from when making a prediction

        VideoWriter outputVideo;                                        // Open the output
        outputVideo.open("test.avi", 
                static_cast<int>(cap.get(CAP_PROP_FOURCC)),
                cap.get(CAP_PROP_FPS), 
                Size((int) cap.get(CAP_PROP_FRAME_WIDTH), 
                  (int) cap.get(CAP_PROP_FRAME_HEIGHT)), true);

        if (!outputVideo.isOpened())
        {
                cout  << "Could not open the output video for write " << endl;
                return -1;
        }

	while (cap.read(frame)) {
		frm.MarkFrame();

                detectLines(&frame, outputVideo);

		/*detectObjects(&frame, &centers, max_obj_count);
		focusClosestObject(&closest_obj, &origin, &centers);
		arc.predictNextFrame(&closest_obj);
		updateHeading(&heading, pred, &origin);
		heading_record.push_back(heading);

		printf("\n**************** FRAME %d ****************\n", frame_count);
		printf("    Actual       (%.1f, %.1f)\n", curr->x, curr->y);
		printf("    Prediction   (%.1f, %.1f) [%.1f%%]\n", pred->point.x, pred->point.y, pred->confidence * 100);
		printf("    Avg FPS      %f\n", frm.GetAverageRate());

		if (frame_count % heading_interval == 0) {
			reportAverageHeading(&heading, &heading_record);
			printf("    Avg Heading  [M: %d, A: %d]\n", heading.magnitude, heading.angle);
		}*/

		/*if (show_images) {
			circle(frame, pred->point, pred->radius, Scalar(240, 255, 255), 2);
			imshow("", frame);
		}*/

		if (waitKey(1) >= 0) break;
		frame_count++;
	}

	printf("\n");
	frm.Stop();
	return 0;
}
