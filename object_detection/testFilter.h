#pragma once

#include "iFilter.h"

using namespace cv; //TODO make this go away
class TestFilter : public IFilter
{
public:
	virtual cv::Mat processFrame(IplImage *frame);
};
