#ifndef CONFIDENCE_ARC_HPP
#define CONFIDENCE_ARC_HPP

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

struct Prediction {
	double confidence;
	cv::Point2f point;

	Prediction();
	Prediction(cv::Point2f p, double c);
};

class ConfidenceArc {
	public:
		ConfidenceArc();
		ConfidenceArc(cv::Point2f* prev, cv::Point2f* curr);
		ConfidenceArc(cv::Point2f* prev, cv::Point2f* curr, int num);
		std::vector<cv::Point2f>* getPath();
		Prediction* getPrediction();
		std::vector<double>* getDistanceErrors();
		std::vector<double>* getAngleErrors();
		cv::Point2f* getPrevious();
		cv::Point2f* getCurrent();
		int* getBacktrace();
		void setBacktrace(int num);
		void predictNextFrame(cv::Point2f* current);
		static void predictNextFrame(std::vector<cv::Point2f>* centers, std::vector<ConfidenceArc>* arcs);

	private:
		cv::Point2f prev, curr;
		int backtrace;
		std::vector<double> dist_errors, angle_errors;
		std::vector<cv::Point2f> path;
		Prediction prediction;

		void construct(cv::Point2f* prev, cv::Point2f* curr, int num);
		std::pair<double, double> calcStats(std::vector<double>* collection);
		double sampleError(std::pair<double, double>* stats);
		void recordError();
		void predictNext();
		double calcConf();
};

#endif
