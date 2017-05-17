#include "conf_arc.hpp"
#include <math.h>
#include <random>
#include <utility>
#include <iostream>
#include <vector>

#define OFFSET 0.000001

using namespace std;
using namespace cv;

ConfidenceArc::ConfidenceArc(Point2f* previous, Point2f* current) {
	this->path.push_back(*previous);
	this->path.push_back(*current);
	this->distanceErrors.push_back(0.0);
	this->distanceErrors.push_back(0.0);
	this->angleErrors.push_back(0.0);
	this->angleErrors.push_back(0.0);
	this->predictNext();
}

vector<Point2f>* ConfidenceArc::getPath() {
	return &(this->path);
}

Prediction* ConfidenceArc::getPrediction() {
	return &(this->prediction);
}

vector<double>* ConfidenceArc::getDistanceErrors() {
	return &(this->distanceErrors);
}

vector<double>* ConfidenceArc::getAngleErrors() {
	return &(this->angleErrors);
}

pair<double, double> ConfidenceArc::calculateStats(vector<double>* collection, int length) {
	double mean = 0, variance = 0;
	int size = collection->size();
	length = length <= 0 ? size : min(length, size);

	for (int i = 1; i <= length; i++) {
		mean += collection->at(size - i) / length;
	}

	for (int i = 1; i <= length; i++) {
		variance += pow(collection->at(size - i) - mean, 2);
	}

	return make_pair(sqrt(variance / (length <= 1 ? 1 : length - 1)), mean);
}

double ConfidenceArc::sampleError(pair<double, double>* stats) {
	default_random_engine sample;
	normal_distribution<double> distribution(stats->second, stats->first);
	return fabs(distribution(sample));
}

void ConfidenceArc::cycleFrame(Point2f* current) {
	this->path.push_back(*current);
	this->recordError();
	this->predictNext();
}

void ConfidenceArc::cycleFrame(vector<Point2f>* centers, vector<ConfidenceArc>* arcs) {
	for (int i = 0; i < centers->size(); i++) {
		arcs->at(i).cycleFrame(&(centers->at(i)));
	}
}

void ConfidenceArc::recordError() {
	Point2f previous = this->path.at(this->path.size() - 2);
	Point2f current = this->path.at(this->path.size() - 1) - previous;
	Point2f predicted = this->prediction.point - previous;
	Point2f error = predicted - current;

	this->distanceErrors.push_back(norm(predicted) - norm(current));
	this->angleErrors.push_back(atan2(error.y, error.x));
}

void ConfidenceArc::predictNext() {
	Point2f previous = this->path.at(this->path.size() - 2);
	Point2f current = this->path.at(this->path.size() - 1);
	
	this->prediction = Prediction(2 * current - previous, this->calculateConfidence());
}

double ConfidenceArc::calculateConfidence() {
	int length = 10;

	Point2f previous = this->path.at(this->path.size() - 2);
	Point2f current = this->path.at(this->path.size() - 1);
	pair<double, double> distanceStats = this->calculateStats(&(this->distanceErrors), length);
	pair<double, double> angleStats = this->calculateStats(&(this->angleErrors), length);

	double distance = norm(current - previous) + OFFSET;
	double distanceError = this->sampleError(&distanceStats) + OFFSET;
	double angleError = this->sampleError(&angleStats) + OFFSET;

	double arcArea = 4 * angleError * distance * distanceError;
	double circleArea = M_PI * pow(distance + distanceError, 2);

	return 1 - arcArea / circleArea;
}
