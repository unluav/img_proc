#include "videoPlayer.h"

VideoPlayer::VideoPlayer(IFilter *_filter, std::string *_videoName, std::string *_windowName){

	if(_filter == NULL){
		throw std::invalid_argument("\n\n\nBAD FILTER\n\n\n");
	}else if(_videoName == NULL){
		throw std::invalid_argument("\n\n\nBAD VIDEO NAME\n\n\n");
	}else if(_windowName == NULL){
		throw std::invalid_argument("\n\n\nBAD WINDOW NAME\n\n\n");
	}


	filter = _filter;
	videoName = _videoName;
	windowName = _windowName;

        video = new cv::VideoCapture(*videoName);

        /* check file was correctly opened */
        if (!video->isOpened()) {
                printf("Unable to open \"%s\"\n", _videoName->c_str());
		throw std::invalid_argument("Error in VideoPlayer Constructor");
        }
}


void VideoPlayer::play(){

	cv::Mat frame;
	bool hasNext = video->read(frame);

        cv::namedWindow(*windowName, 1);

	int frmCounter = 0;
	double speedMultiplier = 1;
	bool reverse = false;

	while (hasNext) {
		//Process key comands
		int key = cvWaitKey(10)%256;
		switch(key) {
			//in the END, it doesn't even matter...
			case 'q':
				printf("Exiting...\n");
				return;
			//STOP. hammer time
			case ' ':
				printf("Paused, press 'p' to resume playback.\n");
				while(cvWaitKey(1)%256 != 'p');
				break;
			//harder, better, FASTER, stronger
			case 'f':
				speedMultiplier *= 2;
				printf("Current multiplier: %.2fx\n", speedMultiplier);
				break;
			//some song with SLOW in the lyrics
			case 's':
				speedMultiplier = (speedMultiplier * .5 >= 1) ? speedMultiplier * .5 : 1;
				printf("Current multiplier: %.2fx\n", speedMultiplier);
				if (speedMultiplier == 0)
					speedMultiplier = 1;
				break;
			//reverse reverse (everybody clap your hands)
			case 'r':
				if (reverse)
					printf("Reverse, reverse! (Forward)\n");
				else
					printf("Reverse, reverse! (Backward)\n");
				reverse = !reverse;
				break;
		}	

		std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << video->get(CV_CAP_PROP_POS_MSEC)  << "ms\r";
		std::cout.flush();

		frmCounter++;
		if (!reverse) {
			if(frmCounter%(int)speedMultiplier != 0){
				double fNum = video->get(CV_CAP_PROP_POS_FRAMES) + 1;
				if (fNum < 0)
					fNum = 0;
				video->set(CV_CAP_PROP_POS_FRAMES, fNum);
				continue;
			}	
		} else {
			if(frmCounter%(int)speedMultiplier != 0) {
				double fNum = video->get(CV_CAP_PROP_POS_FRAMES) - 2;
				if (fNum < 0)
					fNum = 0;
				video->set(CV_CAP_PROP_POS_FRAMES, fNum);
				continue;
			}
		}

		cv::Mat disp = filter->processFrame(frame);

		// show loaded frame 
		cv::imshow(*windowName, disp);

		// load and check next frame
		if (!reverse)
			hasNext = video->read(frame);
		else {
			double fNum = video->get(CV_CAP_PROP_POS_FRAMES) - 2;
			if (fNum < 0)
				fNum = 0;
			video->set(CV_CAP_PROP_POS_FRAMES, fNum);

			hasNext = video->read(frame);
		} 
    	}
}
