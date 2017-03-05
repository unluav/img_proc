#include "videoPlayer.h"

VideoPlayer::VideoPlayer(IFilter *_filter, char *_videoName, char *_windowName){

	if(_videoName != NULL){
		printf("\n\nOMG IT AINT NULL! ... %s\n\n\n", _videoName);
	}

	if(_filter == NULL){
		throw std::invalid_argument("\n\n\nBAD FILTER\n\n\n");
	}else if(_videoName == NULL){
		throw std::invalid_argument("\n\n\nBAD VIDEO NAME\n\n\n");
	}else if(_windowName == NULL){
		throw std::invalid_argument("\n\n\nBAD WINDOW NAME\n\n\n");
	}

/*	if(!_filter || !_videoName || !_windowName){
		printf("Error. Usage: Filter, VideoName, WindowName");
		throw std::invalid_argument("Improper values in VideoPlayer constructor\n");
	}
*/

	filter = _filter;
	videoName = (char *) malloc(sizeof(char) * (strlen(_videoName)+1));
	strcpy(videoName, _videoName);
	windowName = (char *) malloc(sizeof(char) * (strlen(_windowName)+1));
	strcpy(windowName, _windowName);

        video = cvCaptureFromFile(_videoName);

        /* check file was correctly opened */
        if (!video) {
                printf("Unable to open \"%s\"\n", _videoName);
		throw std::invalid_argument("Error in VideoPlayer Constructor");
        }


}


void VideoPlayer::play(){

	IplImage *frame = cvQueryFrame(video);
        namedWindow(windowName, 1);

	int frmCounter = 0;
	double speedMultiplier = 1;
	bool reverse = false;

	while (frame) {
		/*Process key comands*/ 
		int key = cvWaitKey(10)%256;
		switch(key) {
			/*in the END, it doesn't even matter...*/
			case 'q':
				printf("Exiting...\n");
				return;
			/*STOP. hammer time*/
			case ' ':
				printf("Paused, press 'p' to resume playback.\n");
				while(cvWaitKey(1)%256 != 'p');
				break;
			/*harder, better, FASTER, stronger*/
			case 'f':
				speedMultiplier *= 2;
				printf("Current multiplier: %.2fx\n", speedMultiplier);
				break;
			/*some song with SLOW in the lyrics*/
			case 's':
				speedMultiplier = (speedMultiplier * .5 >= 1) ? speedMultiplier * .5 : 1;
				printf("Current multiplier: %.2fx\n", speedMultiplier);
				if (speedMultiplier == 0)
					speedMultiplier = 1;
				break;
			/*reverse reverse (everybody clap your hands)*/
			case 'r':
				if (reverse)
					printf("Reverse, reverse! (Forward)\n");
				else
					printf("Reverse, reverse! (Backward)\n");
				reverse = !reverse;
				break;
		}	

		cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << int(cvGetCaptureProperty(video,CV_CAP_PROP_POS_MSEC) ) << "ms\r";
		cout.flush();

		frmCounter++;
		if (!reverse) {
			if(frmCounter%(int)speedMultiplier != 0){
				double fNum = cvGetCaptureProperty(video, CV_CAP_PROP_POS_FRAMES) + 1 ;
				if (fNum < 0)
					fNum = 0;
				cvSetCaptureProperty(video, CV_CAP_PROP_POS_FRAMES,fNum);
				continue;
			}	
		} else {
			if(frmCounter%(int)speedMultiplier != 0) {
				double fNum = cvGetCaptureProperty(video, CV_CAP_PROP_POS_FRAMES) - 2;
				if (fNum < 0)
					fNum = 0;
				cvSetCaptureProperty(video, CV_CAP_PROP_POS_FRAMES,fNum);
				continue;
			}
		}

		Mat disp = filter->processFrame(frame);

		/* show loaded frame */
		imshow(windowName, disp);

		/* load and check next frame*/
		if (!reverse)
			frame = cvQueryFrame(video);
		else {
			double fNum = cvGetCaptureProperty(video, CV_CAP_PROP_POS_FRAMES) - 2;
			if (fNum < 0)
				fNum = 0;
			cvSetCaptureProperty(video, CV_CAP_PROP_POS_FRAMES,fNum);
			frame = cvQueryFrame(video);
		} 
		if(!frame) {
		printf("error loading frame.\n");
			return;
		}
		
    	}
	

}
