#include "ConfidenceArc.hpp"
#include <math.h>
#include <random>
#include <utility>

void ConfidenceArc::predictPoint(cv::Point2f prev, cv::Point2f curr,
	std::pair<double, double>* distStats, std::pair<double, double>* angleStats,
	std::vector<double>* distErrors, std::vector<double>* angleErrors, Prediction* prediction) {

	std::default_random_engine generator;
	std::normal_distribution<double> randDist(distStats->first, distStats->second);
	std::normal_distribution<double> randAngle(angleStats->first, angleStats->second);

	cv::Point2f diff = curr - prev;
	double dist = cv::norm(diff);
	double angle = atan2(diff.y, diff.x);
	double distError = fabs(randDist(generator));
	double angleError = fabs(randAngle(generator));

	distErrors->push_back(prediction->dist - dist);
	angleErrors->push_back(prediction->angle - angle);
	ConfidenceArc::getErrorStats(distErrors, distStats, 10);
	ConfidenceArc::getErrorStats(angleErrors, angleStats, 10);

	prediction->dist = dist;
	prediction->angle = angle;
	prediction->distError = distError;
	prediction->angleError = angleError;
}

void ConfidenceArc::getErrorStats(std::vector<double>* errors, std::pair<double, double>* unit, int cap) {
	unit->first = 0;
	for (int i = 1; i <= cap; i++) {
		int index = errors->size() - i;
		if (index < 0) break;
		unit->first += errors->at(index) / errors->size();
	}

	double variance = 0;
	for (int i = 1; i <= cap; i++) {
		int index = errors->size() - i;
		if (index < 0) break;
		variance += pow(errors->at(index) - unit->first, 2);
	}

	unit->second = sqrt(variance / (errors->size() - 1));
}

double ConfidenceArc::getConfidence(double distance, double distanceError, double angleError) {
	double outer = distance + distanceError;
	double arcArea = 4 * angleError * distance * distanceError;
	double circleArea = M_PI * outer * outer;
	double confidence;

	if (std::isnan(distanceError) || std::isnan(angleError)) confidence = 1;
	else if (outer == 0) confidence = 0;
	else if (distance < distanceError) confidence = 2;
	else if (distanceError == 0 && angleError != 0) confidence = angleError / M_PI;
	else if (angleError == 0) confidence = 2 * distanceError / outer;
	else confidence = arcArea / circleArea;

	return 1 - confidence;
}
