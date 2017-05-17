#include "conf_arc.hpp"

using namespace std;
using namespace cv;

void printFrame(vector<Point2f>* centers, vector<ConfidenceArc>* arcs) {
	for (int i = 0; i < centers->size(); i++) {
		Point2f center = centers->at(i);
		Prediction* prediction = arcs->at(i).getPrediction();
		printf("A: (%.3f, %.3f)\t->\t", center.x, center.y);
		printf("P: (%.3f, %.3f)\t", prediction->point.x, prediction->point.y);
		printf("[ %.6f%% ]\n", prediction->confidence * 100);
	}
}

void getNextFrame(vector<Point2f>* centers, double randCap, int frame) {
	for (int i = 0; i < centers->size(); i++) {
		centers->at(i).x = frame + i + (double) rand() / RAND_MAX * randCap;
		centers->at(i).y = frame + i + (double) rand() / RAND_MAX * randCap;
	}
}

int main(int argc, char** argv) {
	int roombaCount = 10;
	vector<Point2f> centers;
	vector<ConfidenceArc> arcs;
	int frames = 10;
	double randCap = 0.01;
	Point2f origin(0.0f, 0.0f);

	for (int i = 0; i < roombaCount; i++) {
		centers.push_back(origin);
		arcs.push_back(ConfidenceArc(&origin, &origin));
	}

	for (int i = 0; i < frames; i++) {
		getNextFrame(&centers, randCap, i);
		printf("\n\nWMWMWMWMWMWMWMWMWMWMWMWM FRAME %d MWMWMWMWMWMWMWMWMWMWMWMW\n", i + 1);
		printFrame(&centers, &arcs);
		ConfidenceArc::cycleFrame(&centers, &arcs);
	}

	return 0;
}
