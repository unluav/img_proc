using namespace std;
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "iFilter.h"

using namespace cv;

class VideoPlayer{

	public:

		VideoPlayer(IFilter *_filter, char *_videoName, char *_windowName);
		void play();

	private:

		IFilter *filter;
		char *videoName;
		char *windowName;
		CvCapture *video;
};
