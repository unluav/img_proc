#include <iostream>
#include <string>
#include <math.h>
#include "ConfidenceArc.hpp"

int main() {
	double distanceError, thetaError;
	cv::Point2f predicted(0.f, 0.f), previous(0.f, 0.f);

	for (int i = 0; i < 10; i++) {
		float x = i + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.1));
		float y = i + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.1));
		cv::Point2f current(x, y);
		distanceError = sqrt(pow(current.x - predicted.x, 2) + pow(current.y - predicted.y, 2));
		thetaError = atan2(current.y, current.x) - atan2(predicted.y, predicted.y);
		ConfidenceArc arc(previous, current, thetaError, distanceError);
		std::cout << thetaError << " " << distanceError << std::endl;
		std::cout << "Theta Confidence: " << arc.getThetaConfidence() << std::endl;
		std::cout << "Distance Confidence: " << arc.getDistanceConfidence() << std::endl << std::endl;
		previous = current;
		predicted = arc.predictPoint();
	}

	return 0;
}

void predictPoint(, cv::Point previous, cv::Point current, double thetaError, double distanceError) {
	
}
