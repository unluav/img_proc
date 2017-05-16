#include <iostream>
#include <iomanip>
#include "confidence.hpp"
#include <random>

void printStage(cv::Point2f* previous, cv::Point2f* current, Prediction* prediction) {
	printf("\n\n\nPrevious: (%.3f, %.3f)\n", previous->x, previous->y);
	printf("Current: (%.3f, %.3f)\n", current->x, current->y);
	printf("Predicted Point: (%.3f, %.3f)\n", prediction->point.x, prediction->point.y);
	printf("Confidence: %.3f\n", prediction->confidence);
}

//Expected arguments are 1)Number of predictions and 2)Desired randomness of movement (0-1)
int main(int argc, char** argv) {
	cv::Point2f previous(0, 0);
	cv::Point2f current(0, 0);
	ConfidenceArc arc(&previous, &current);
	std::vector<cv::Point2f>* path = arc.getPath();
	
	for (int i = 0; i < atoi(argv[1]); i++) {
		current.x = i + (double) rand() / RAND_MAX * atof(argv[2]);
		current.y = i + (double) rand() / RAND_MAX * atof(argv[2]);
		printStage(&previous, &current, arc.getLatestPrediction());

		arc.recordPoint(&current);
		arc.recordError(&previous, &current);
		arc.predictPoint(10);
		previous = current;
	}

	for (int i = 0; i < path->size(); i++) {
		printf("Pt: (%.3f, %.3f)\n", path->at(i).x, path->at(i).y);
	}

//Godzilla says I should learn to fight my own battles!!!!
//"we're so heavy in code... mechanical engineers don't know code at all - Daric"
//this is the ultimate showdown. of ultimate destiny. good guys bad guys and explosison. as far as the eye can see.	

	return 0;
}
