#include "confidence.hpp"
#include <math.h>
#include <random>
#include <utility>
#include <iostream>
#include <vector>

#define OFFSET 0.000001

ConfidenceArc::ConfidenceArc(cv::Point2f* p1, cv::Point2f* p2) {
	this->recordPoint(p1);
	this->recordPoint(p2);
	this->distanceErrors.push_back(0.0);
	this->distanceErrors.push_back(0.0);
	this->angleErrors.push_back(0.0);
	this->angleErrors.push_back(0.0);
}

void ConfidenceArc::recordPoint(cv::Point2f* p) {
	this->path.push_back(*p);
}

std::vector<cv::Point2f>* ConfidenceArc::getPath() {
	return &(this->path);
}

void ConfidenceArc::predictPoint(Prediction* prediction, int length) {
	// Calculates the standard deviation and mean from the previous distance and angle errors
	std::pair<double, double> distanceStats = this->fetchDevAndMean(&(this->distanceErrors), length);
	std::pair<double, double> angleStats = this->fetchDevAndMean(&(this->angleErrors), length);

	// Samples the normal distribution formed by the standard deviation and mean
	double distanceError = this->fetchError(&distanceStats);
	double angleError = this->fetchError(&angleStats);

	// Straight-line prediction of the next point
	cv::Point2f previous = this->path.at(this->path.size() - 2);
	cv::Point2f current = this->path.at(this->path.size() - 1);
	cv::Point2f difference = current - previous;

	// Calculates the confidence
	prediction->point = current + difference;
	prediction->confidence = this->fetchConfidence(cv::norm(difference), distanceError, angleError);
}

double ConfidenceArc::fetchConfidence(double distance, double distanceError, double angleError) {
	distance += OFFSET;
	distanceError += OFFSET;
	angleError += OFFSET;

	// Maths
	double arcArea = 4 * angleError * distance * distanceError;
	double circleArea = M_PI * pow(distance + distanceError, 2);

	return 1 - arcArea / circleArea;
}

std::pair<double, double> ConfidenceArc::fetchDevAndMean(std::vector<double>* collection, int length) {
	// Hand wavy statistical calculations
	double mean = 0, variance = 0;
	int size = collection->size();
	length = length < 0 ? size : std::min(length, size);

	for (int i = 1; i <= length; i++) {
		mean += collection->at(size - i) / length;
	}

	for (int i = 1; i <= length; i++) {
		variance += pow(collection->at(size - i) - mean, 2);
	}

	return std::make_pair(sqrt(variance / (length <= 1 ? 1 : length - 1)), mean);
}

double ConfidenceArc::fetchError(std::pair<double, double>* stats) {
	// More hand wavy statistical calculations
	std::default_random_engine sample;
	std::normal_distribution<double> distribution(stats->second, stats->first);
	return fabs(distribution(sample));
}

void ConfidenceArc::recordError(cv::Point2f* previous, cv::Point2f* current, cv::Point2f* prediction) {
	// Frames all three points in terms of previous being treated as the origin
	cv::Point2f predicted = *prediction - *previous;
	cv::Point2f actual = *current - *previous;

	this->distanceErrors.push_back(fabs(cv::norm(predicted) - cv::norm(actual)));
	this->angleErrors.push_back(fabs(atan2(predicted.y - actual.y, predicted.x - actual.x)));
}
