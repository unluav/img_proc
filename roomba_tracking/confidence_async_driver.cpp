#include <iostream>
#include <iomanip>
#include "ConfidenceArc.hpp"
#include <random>
#include <pthread.h>

void* trackRoomba(void* arg) {
	Prediction prediction;
	cv::Point2f previous(0, 0);
	cv::Point2f current(0, 0);
	ConfidenceArc arc(&previous, &current);
	std::vector<cv::Point2f>* path = arc.getPath();
	
	for (int i = 0; i < 10; i++) {
		current.x = i + (double) rand() / RAND_MAX * 0.01;
		current.y = i + (double) rand() / RAND_MAX * 0.01;
		printf("%lu: %.3f\n", pthread_self() % 100000000, prediction.confidence);		

		arc.recordPoint(&current);
		arc.recordError(&previous, &current, &(prediction.point));
		arc.predictPoint(&prediction, -1);
		previous = current;
	}

//	for (int i = 0; i < path->size(); i++) {
//		printf("\tPt: (%.3f, %.3f)\n", path->at(i).x, path->at(i).y);
//	}
}

int main(int argc, char** argv) {
	int roombaCount = atoi(argv[1]);
	pthread_t roombas[roombaCount];
	int frames = 10;

	for (int i = 0; i < roombaCount; i++) {
		pthread_create(&roombas[i], NULL, &trackRoomba, NULL);
	}

	for (int i = 0; i < roombaCount; i++) {
		pthread_join(roombas[i], NULL);
	}
}
