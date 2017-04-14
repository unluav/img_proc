#ifndef CONFIDENCE_ARC_HPP
#define CONFIDENCE_ARC_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <queue>
#include <utility>

struct Prediction {
	double confidence;
	cv::Point2f point;

	Prediction() {
		point.x = 0.0f;
		point.y = 0.0f;
		confidence = 0;
	}

	Prediction(cv::Point2f p, double c) {
		point = p;
		confidence = c;
	}
};

class ConfidenceArc {
	public:
		ConfidenceArc(cv::Point2f* previous, cv::Point2f* current);
		void recordPoint(cv::Point2f* p);
		std::vector<cv::Point2f>* getPath();
		std::vector<Prediction>* getPredictionHistory();
		Prediction* getLatestPrediction();
		void predictPoint(int length);
		double fetchConfidence(double distance, double distanceError, double angleError);
		std::pair<double, double> fetchDevAndMean(std::vector<double>* collection, int length);
		double fetchError(std::pair<double, double>* stats);
		void recordError(cv::Point2f* previous, cv::Point2f* current);
		void cyclePoints(cv::Point2f* previous, cv::Point2f* current, int length);

	private:
		std::vector<double> distanceErrors, angleErrors;
		std::vector<cv::Point2f> path;
		std::vector<Prediction> predictionHistory;
};

#endif
