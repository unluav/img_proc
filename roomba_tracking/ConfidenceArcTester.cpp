#include <iostream>
#include <string>
#include "ConfidenceArc.hpp"
#include <utility>

void Print(cv::Point2f prev, cv::Point2f curr, Prediction prediction);

int main() {
	std::vector<cv::Point2f> path;
	std::vector<double> distErrors;
	std::vector<double> angleErrors;
	cv::Point2f prev(0.0, 0.0);
	cv::Point2f curr;
	Prediction prediction;
	std::pair<double, double> distStats = std::make_pair(0.0, 0.0);
	std::pair<double, double> angleStats = std::make_pair(0.0, 0.0);
	
	std::cout << "Iterations: ";
	int cap;
	std::cin >> cap;

	for (int i = 0; i < cap; i++) {
		curr.x = i + (double) rand() / RAND_MAX;
		curr.y = i + (double) rand() / RAND_MAX;

		path.push_back(prev);
		ConfidenceArc::predictPoint(prev, curr, &distStats, &angleStats, &distErrors, &angleErrors, &prediction);

		std::cout << i << std::endl;
		Print(prev, curr, prediction);

		prev = curr;
	}

	return 0;
}

void Print(cv::Point2f prev, cv::Point2f curr, Prediction prediction) {
	std::cout << "(" << prev.x << ", " << prev.y << ") -> ";
	std::cout << "(" << curr.x << ", " << curr.y << ")" << std::endl;
	std::cout << "Distance: " << prediction.dist << std::endl;
	std::cout << "Angle: " << prediction.angle << std::endl;
	std::cout << "Distance Error: " << prediction.distError << std::endl;
	std::cout << "Angle Error: " << prediction.angleError << std::endl;
	std::cout << "Confidence: " << ConfidenceArc::getConfidence(prediction.dist, prediction.distError, prediction.angleError);
	std::cout << std::endl << std::endl << std::endl;
}
