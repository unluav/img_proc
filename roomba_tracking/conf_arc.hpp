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
		point = Point2f(0.0f, 0.0f);
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
		vector<Point2f>* getPath();
		Prediction* getPrediction();
		vector<double>* getDistanceErrors();
		vector<double>* getAngleErrors();
		pair<double, double> calculateStats(vector<double>* collection, int length);
		double sampleError(pair<double, double>* stats);
		void cycleFrame(Point2f* current);
		static void cycleFrame(vector<Point2f>* centers, vector<ConfidenceArc>* arcs);
		void recordError();
		void predictNext();
		double calculateConfidence();

	private:
		vector<double> distanceErrors, angleErrors;
		vector<Point2f> path;
		Prediction prediction;
};

#endif
