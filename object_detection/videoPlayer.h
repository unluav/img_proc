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

		VideoPlayer(IFilter *_filter, string *_videoName, string *_windowName);
		void play();

	private:

		IFilter *filter;
		string *videoName;
		string *windowName;
		VideoCapture *video;
};
