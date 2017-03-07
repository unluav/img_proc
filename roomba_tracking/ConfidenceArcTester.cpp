#include <iostream>
#include <string>
#include "ConfidenceArc.hpp"
#include <utility>

int main() {
	std::vector<cv::Point2f> path;
	std::vector<double> distanceErrors;
	std::vector<double> angleErrors;
	cv::Point2f previous(0.0, 0.0);
	cv::Point2f current;
	Prediction prediction;
	std::pair<double, double> distanceStats = std::make_pair(0.0, 0.0);
	std::pair<double, double> angleStats = std::make_pair(0.0, 0.0);
	
	std::cout << "Iterations: ";
	int cap;
	std::cin >> cap;

	for (int i = 0; i < cap; i++) {
		current.x = i + (double) rand() / RAND_MAX;
		current.y = i + (double) rand() / RAND_MAX;

		path.push_back(previous);
		ConfidenceArc::getPredictedPoint(previous, current,
			&distanceStats, &angleStats,
			&distanceErrors, &angleErrors, &prediction);

		std::cout << i << std::endl;
		std::cout << "(" << previous.x << ", " << previous.y << ") -> ";
		std::cout << "(" << current.x << ", " << current.y << ")" << std::endl;
		std::cout << "Distance: " << prediction.distance << std::endl;
		std::cout << "Angle: " << prediction.angle << std::endl;
		std::cout << "Distance Error: " << prediction.distanceError << std::endl;
		std::cout << "Angle Error: " << prediction.angleError << std::endl;
		std::cout << "Confidence: " << ConfidenceArc::getConfidence(prediction.distance, prediction.distanceError, prediction.angleError);
		std::cout << std::endl << std::endl << std::endl;
		previous = current;
	}

	return 0;
}
