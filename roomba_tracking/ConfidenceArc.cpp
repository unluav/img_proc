#include "ConfidenceArc.hpp"
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

std::vector<cv::Point2f> ConfidenceArc::getPath() {
	return this->path;
}

void ConfidenceArc::predictPoint(Prediction* prediction, int length) {
	std::pair<double, double> distanceStats = this->fetchDevAndMean(&(this->distanceErrors), length);
	std::pair<double, double> angleStats = this->fetchDevAndMean(&(this->angleErrors), length);

	double distanceError = this->fetchError(&distanceStats);
	double angleError = this->fetchError(&angleStats);

	cv::Point2f previous = this->path.at(this->path.size() - 2);
	cv::Point2f current = this->path.at(this->path.size() - 1);
	cv::Point2f difference = current - previous;

	prediction->point = current + difference;
	prediction->confidence = this->fetchConfidence(cv::norm(difference), distanceError, angleError);
}

double ConfidenceArc::fetchConfidence(double distance, double distanceError, double angleError) {
	distance += OFFSET;
	distanceError += OFFSET;
	angleError += OFFSET;

//	std::cout << "distance := " << distance << std::endl;
//	std::cout << "distanceError := " << distanceError << std::endl;
//	std::cout << "angleError := " << angleError << std::endl;

	double arcArea = 4 * angleError * distance * distanceError;
	double circleArea = M_PI * pow(distance + distanceError, 2);

//	std::cout << "arcArea := " << arcArea << std::endl;
//	std::cout << "circleArea := " << circleArea << std::endl;

	return 1 - arcArea / circleArea;
}

std::pair<double, double> ConfidenceArc::fetchDevAndMean(std::vector<double>* collection, int length) {
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
	std::default_random_engine sample;
	std::normal_distribution<double> distribution(stats->second, stats->first);
	return fabs(distribution(sample));
}

void ConfidenceArc::recordError(cv::Point2f* previous, cv::Point2f* current, cv::Point2f* prediction) {
	cv::Point2f predicted = *prediction - *previous;
	cv::Point2f actual = *current - *previous;

	this->distanceErrors.push_back(fabs(cv::norm(predicted) - cv::norm(actual)));
	this->angleErrors.push_back(fabs(atan2(predicted.y - actual.y, predicted.x - actual.x)));

//	std::cout << "\n\nERRORS\n" << std::endl;
//	std::cout << "Dist\tAngle" << std::endl;
//	for (int i = 0; i < distanceErrors.size(); i++) {
//		std::cout << distanceErrors.at(i) << "\t" << angleErrors.at(i) << std::endl;
//	}
//	std::cout << std::endl;
}
