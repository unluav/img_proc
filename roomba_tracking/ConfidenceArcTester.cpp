#include <iostream>
#include <iomanip>
#include "ConfidenceArc.hpp"
#include <random>

void printStage(cv::Point2f* previous, cv::Point2f* current, Prediction* prediction) {
	printf("\n\n\nPrevious: (%.3f, %.3f)\n", previous->x, previous->y);
	printf("Current: (%.3f, %.3f)\n", current->x, current->y);
	printf("Predicted Point: (%.3f, %.3f)\n", prediction->point.x, prediction->point.y);
	printf("Confidence: %.3f\n", prediction->confidence);
}

int main(int argc, char** argv) {
	Prediction prediction;
	cv::Point2f previous(0, 0);
	cv::Point2f current(0, 0);
	ConfidenceArc arc(&previous, &current);
	
	for (int i = 0; i < atoi(argv[1]); i++) {
		current.x = i + (double) rand() / RAND_MAX * atof(argv[2]);
		current.y = i + (double) rand() / RAND_MAX * atof(argv[2]);
		arc.recordPoint(&current);
		printStage(&previous, &current, &prediction);
		arc.recordError(&previous, &current, &(prediction.point));
		arc.predictPoint(&prediction, 10);
		previous = current;
	}

//	std::vector<cv::Point2f> path = arc.getPath();
//	for (int i = 0; i < path.size(); i++) {
//		printf("Pt: (%.3f, %.3f)\n", path.at(i).x, path.at(i).y);
//	}	

	return 0;
}
