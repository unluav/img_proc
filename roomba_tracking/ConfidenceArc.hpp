#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <queue>
#include <utility>

struct Prediction {
	double dist, angle, distError, angleError;

	Prediction() {
		dist = 0;
		angle = 0;
		distError = 0;
		angleError = 0;
	}
};

class ConfidenceArc {
	public:
		static void predictPoint(cv::Point2f prev, cv::Point2f curr,
			std::pair<double, double>* distStats, std::pair<double, double>* angleStats,
			std::vector<double>* distErrors, std::vector<double>* angleErrors, Prediction* prediction);
		static void getErrorStats(std::vector<double>* errors, std::pair<double, double>* unit, int cap);
		static double getConfidence(double distance, double distanceError, double angleError);
};
