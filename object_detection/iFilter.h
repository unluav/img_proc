#pragma once

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

using namespace cv; //TODO: EW EW EW
//This is an abstract class for a filter object, which will be used in conjunction with the video player

class IFilter {

	public:
		virtual cv::Mat processFrame(Mat& frame) = 0;
};
