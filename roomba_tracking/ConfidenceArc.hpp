#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <queue>
#include <utility>

struct Prediction {
	double distance, angle, distanceError, angleError;

	Prediction() {
		distance = 0;
		angle = 0;
		distanceError = 0;
		angleError = 0;
	}
};

class ConfidenceArc {
	public:
		static void getPredictedPoint(cv::Point2f previous, cv::Point2f current,
			std::pair<double, double>* distanceStats, std::pair<double, double>* angleStats,
			std::vector<double>* distanceErrors, std::vector<double>* angleErrors, Prediction* prediction);
		static void getMeasurementError(std::vector<double>* errors, std::pair<double, double>* measurement);
		static double getConfidence(double distance, double distanceError, double angleError);
};
