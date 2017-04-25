#ifndef CONFIDENCE_ARC_HPP
#define CONFIDENCE_ARC_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <queue>
#include <utility>

using namespace std;
using namespace cv;

struct Prediction {
	double confidence;
	Point2f point;

	Prediction() {
		point.x = 0.0f;
		point.y = 0.0f;
		confidence = 0;
	}

	Prediction(Point2f p, double c) {
		point = p;
		confidence = c;
	}
};

class ConfidenceArc {
	public:
		ConfidenceArc(Point2f* previous, Point2f* current);
		void recordPoint(Point2f* p);
		vector<Point2f>* getPath();
		vector<Prediction>* getPredictionHistory();
		Prediction* getLatestPrediction();
		void predictPoint(int length);
		double fetchConfidence(double distance, double distanceError, double angleError);
		pair<double, double> fetchDevAndMean(vector<double>* collection, int length);
		double fetchError(pair<double, double>* stats);
		void recordError(Point2f* previous, Point2f* current);
		void cyclePoints(Point2f* previous, Point2f* current, int length);

	private:
		vector<double> distanceErrors, angleErrors;
		vector<Point2f> path;
		vector<Prediction> predictionHistory;
};

#endif
