#include "conf_arc.hpp"

using namespace std;
using namespace cv;

#define RAND_CAP 0.01

// Just a helpful printer
void printFrame(vector<Point2f>* centers, vector<ConfidenceArc>* arcs, int frame) {
	printf("\n\n************************ FRAME %d ************************\n", frame);
	
	for (int i = 0; i < centers->size(); i++) {
		Prediction* pred = (*arcs)[i].getPrediction();
		printf("A: (%.3f, %.3f)\t->\t", (*centers)[i].x, (*centers[i]).y);
		printf("P: (%.3f, %.3f)\t[ %.6f%% ]\n", pred->point.x, pred->point.y, pred->confidence * 100);
	}
}

// Pseudo-randomly generates the next frame
// More of a placeholder for passing in a frame to ConfidenceArc
void getNextFrame(vector<Point2f>* centers, int frame) {
	for (int i = 0; i < centers->size(); i++) {
		(*centers)[i].x = frame + i + (double) rand() / RAND_MAX * RAND_CAP;
		(*centers)[i].y = frame + i + (double) rand() / RAND_MAX * RAND_CAP;
	}
}

int main(int argc, char** argv) {
	int obj_count = 1, frames = 3;
	vector<Point2f> centers(obj_count);
	vector<ConfidenceArc> arcs(obj_count);

	for (int i = 0; i < frames; i++) {
		getNextFrame(&centers, i);
		printFrame(&centers, &arcs, i + 1);
		ConfidenceArc::predictNextFrame(&centers, &arcs);
	}

	return 0;
}
