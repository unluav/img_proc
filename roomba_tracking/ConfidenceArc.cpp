#include "ConfidenceArc.hpp"
#include <math.h>
#include <iostream>
#include <random>
#include <utility>

void ConfidenceArc::getPredictedPoint(cv::Point2f previous, cv::Point2f current,
	std::pair<double, double>* distanceStats, std::pair<double, double>* angleStats,
	std::vector<double>* distanceErrors, std::vector<double>* angleErrors, Prediction* prediction) {

	std::default_random_engine generator;
	std::normal_distribution<double> distanceDistribution(distanceStats->first, distanceStats->second);
	std::normal_distribution<double> angleDistribution(angleStats->first, angleStats->second);

	cv::Point2f difference = current - previous;
	double distance = cv::norm(difference);
	double angle = atan2(difference.y, difference.x);
	double distanceError = fabs(distanceDistribution(generator));
	double angleError = fabs(angleDistribution(generator));

	distanceErrors->push_back(prediction->distance - distance);
	angleErrors->push_back(prediction->angle - angle);

	ConfidenceArc::getMeasurementError(distanceErrors, distanceStats);
	ConfidenceArc::getMeasurementError(angleErrors, angleStats);

	prediction->distance = distance;
	prediction->angle = angle;
	prediction->distanceError = distanceError;
	prediction->angleError = angleError;
}

void ConfidenceArc::getMeasurementError(std::vector<double>* errors, std::pair<double, double>* measurement) {
	measurement->first = 0;
	for (int i = 1; i <= 10; i++) {
		int index = errors->size() - i;
		if (index < 0) break;
		measurement->first += errors->at(index);
	}

	measurement->first = fabs(measurement->first) / errors->size();

	double variance = 0;
	for (int i = 1; i <= 10; i++) {
		int index = errors->size() - i;
		if (index < 0) break;
		variance += pow(errors->at(index) - measurement->first, 2);
	}

	measurement->second = fabs(sqrt(variance / (errors->size() - 1)));
}

double ConfidenceArc::getConfidence(double distance, double distanceError, double angleError) {
	double outer = distance + distanceError;
	double arcArea = 4 * angleError * distance * distanceError;
	double circleArea = M_PI * outer * outer;

	if (outer == 0) return 0;
	else if (distanceError == 0 && angleError != 0) return 1 - angleError / M_PI;
	else if (distance < distanceError) return -1;
	else if (angleError == 0) return 1 - 2 * distanceError / outer;
	else return 1 - arcArea / circleArea;
}
