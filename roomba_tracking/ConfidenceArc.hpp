#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>

class ConfidenceArc {
	public:
		ConfidenceArc(cv::Point2f p1, cv::Point2f p2, double thetaError, double distanceError);
		double getTheta();
		double getDistance();
		double getThetaError();
		double getDistanceError();
		double getTotalError();
		double getThetaConfidence();
		double getDistanceConfidence();
		double getTotalConfidence();
		cv::Point2f predictPoint();
		
	private:
		cv::Point2f previous, current;
		double thetaError, distanceError;
};
