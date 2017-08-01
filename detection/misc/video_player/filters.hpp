#pragma once

#include "iFilter.hpp"

class TestFilter : public IFilter
{
public:
	virtual cv::Mat processFrame(cv::Mat& frame);
};