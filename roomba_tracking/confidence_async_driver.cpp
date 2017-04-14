#include <iostream>
#include <iomanip>
#include "confidence.hpp"
#include <random>
#include <pthread.h>

void* trackRoomba(void* arg) {
	cv::Point2f current(0, 0);
	cv::Point2f previous = current;
	ConfidenceArc arc(&previous, &current);
	
	for (int i = 0; i < 10; i++) {
		current.x = i + (double) rand() / RAND_MAX * 0.01;
		current.y = i + (double) rand() / RAND_MAX * 0.01;
		printf("%lu: %.3f\n", pthread_self() % 100000000, arc.getLatestPrediction()->confidence);		

		arc.cyclePoints(&previous, &current, -1);
		previous = current;
	}
}

void recordRoombas(std::vector<cv::Point2f>* centers, std::vector<ConfidenceArc>* arcs) {
	for (int i = 0; i < centers->size(); i++) {
		std::vector<cv::Point2f>* path = arcs->at(i).getPath();
		cv::Point2f previous = path->at(path->size() - 1);
		cv::Point2f current = centers->at(i);

		arcs->at(i).cyclePoints(&previous, &current, -1);
	}
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
