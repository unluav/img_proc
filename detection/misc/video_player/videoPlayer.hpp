#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "iFilter.hpp"


class VideoPlayer{

	public:

		VideoPlayer(IFilter *_filter, std::string *_videoName, std::string *_windowName);
		void play();

	private:

		IFilter *filter;
		std::string *videoName;
		std::string *windowName;
		cv::VideoCapture *video;
};