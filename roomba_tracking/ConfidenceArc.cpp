#include "ConfidenceArc.hpp"
#include <math.h>
#include <iostream>

ConfidenceArc::ConfidenceArc(cv::Point2f p1, cv::Point2f p2, double thetaError, double distanceError) {
	this->previous = p1;
	this->current = p2;
	this->thetaError = thetaError;
	this->distanceError = distanceError;
}

double ConfidenceArc::getThetaError() {
	return this->thetaError;
}

double ConfidenceArc::getDistanceError() {
	return this->distanceError;
}

double ConfidenceArc::getTheta() {
	double x = this->current.x - this->previous.x;
	double y = this->current.y - this->previous.y;
	return abs(atan2(y, x));
}

double ConfidenceArc::getDistance() {
	double xComponent = pow(this->current.x - this->previous.x, 2);
	double yComponent = pow(this->current.y - this->previous.y, 2);
	return sqrt(xComponent + yComponent);
}

double ConfidenceArc::getTotalError() {	
	double distance = this->getDistance();
	double innerRadius = distance - this->distanceError;
	double outerRadius = distance + this->distanceError;
	double innerArea = M_PI * innerRadius * innerRadius;
	double outerArea = M_PI * outerRadius * outerRadius;
	return (outerArea - innerArea) * this->getTheta() / (M_PI * 2);
}

double ConfidenceArc::getThetaConfidence() {
	std::cout << this->thetaError << " " << this->getTheta() << std::endl;
	return this->thetaError / this->getTheta();
}

double ConfidenceArc::getDistanceConfidence() {
	return this->distanceError / this->getDistance();
}

double ConfidenceArc::getTotalConfidence() {
	double outerRadius = this->getDistance() + this->distanceError;
	double outerArea = M_PI * outerRadius * outerRadius;
	return this->getTotalError() / outerArea;
}

cv::Point2f ConfidenceArc::predictPoint() {
	double x = 2 * this->current.x - this->previous.x;
	double y = 2 * this->current.y - this->previous.y;
	cv::Point2f predictedPoint(x, y);
	return predictedPoint;
}
