using namespace std;
#include <stdio.h>
#include <iostream>
#include <map>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
int main(int argc, char *argv[]) {
	int delay = 0, key=0, i=0, counter=0;

	char *window_name;
	CvCapture *video = NULL;
	IplImage  *frame = NULL;
	//IplImage  *grey  = NULL;
	IplImage  *blobs = NULL;
	int threshold_value=50, threshold_type=2;
	int const max_BINARY_value = 255;


	/* check for video file passed by command line */
	if (argc>1) {
		video = cvCaptureFromFile(argv[1]);
	} else {
		printf("Usage: %s VIDEO_FILE\n", argv[0]);
		return 1;
	}

	/* check file was correctly opened */
	if (!video) {
		printf("Unable to open \"%s\"\n", argv[1]);
		return 1;
	}

	/* create a video window with same name of the video file, auto sized */
	window_name = argv[1];
	namedWindow(window_name, 1);

	/* Get the first frame and create a edges image with the same size */
	frame = cvQueryFrame(video);
	//grey  = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	blobs = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);			
	/* calculate the delay between each frame and display video's FPS */
	delay = (int) (1000/cvGetCaptureProperty(video, CV_CAP_PROP_FPS));

	/*these are boolean matrices ... redBlobs2 is because we have to deal with red twice due to the color spectrum linear scale*/	 
	Mat tempFrame, redBlobs1, redBlobs2,greenBlobs;			
	int frmCounter = 0;
	double speedMultiplier = 1;
	bool reverse = false;

	while (frame) {
		/*Process key comands*/ 
		key = cvWaitKey(10)%256;
		switch(key) {
			/*in the END, it doesn't even matter...*/
			case 'q':
				printf("Exiting...\n");
				return 0;
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

		//frm.MarkFrame();  //Daric's frame counter -- check gpu canny edge
		vector<vector<Point> > redContours; 		//vector is a list in c#
		vector<vector<Point> > greenContours;
		vector<Vec4i> rHierarchy;			//hierarchy?
		vector<Vec4i> gHierarchy;

		//Creating a Mat version of the current frame
		cvarrToMat(frame).copyTo(tempFrame);		//copy the crrent frame into a temp frame that we can edit
		cvarrToMat(blobs).copyTo(redBlobs1);		//initializes the matrices of certain size
		cvarrToMat(blobs).copyTo(redBlobs2);
		cvarrToMat(blobs).copyTo(greenBlobs);


		/* Edges on the input gray image (needs to be grayscale) using the Canny algorithm.
		Uses two threshold and a aperture parameter for Sobel operator. */
		cvtColor(tempFrame, redBlobs1, CV_BGR2HSV);	//copies tempFrame into redBlobs with color for edge detection
		cvtColor(tempFrame, redBlobs2, CV_BGR2HSV);	//color filters
		cvtColor(tempFrame, greenBlobs, CV_BGR2HSV);
 
		/*Finding the color blobs (needs to be a colored image) using Inrange*/
		inRange(redBlobs1, Scalar(160,80,80), Scalar(179,255,255),redBlobs1);
		inRange(redBlobs2, Scalar(0,80,80), Scalar(10,255,255),redBlobs2);

		Mat redBlobs;
		bitwise_or(redBlobs1, redBlobs2, redBlobs);
		inRange(greenBlobs, Scalar(40,80,80), Scalar(80,255,255), greenBlobs);
		// 	findContours(redBlobs, redContours, rHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		findContours(greenBlobs, greenContours, gHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		// 	for( int i = 0; i< redContours.size(); i++ )
		//   {
		//     Scalar color = Scalar( 0,255,255);
		//     drawContours( redBlobs, redContours, i, color, 2, 8, rHierarchy, 0, Point() );
		//   }
		for( int i = 0; i< greenContours.size(); i++ )
		{
			Scalar color = Scalar( 120,255,255 );
			//instead of using i, we could use -1, which would draw them all
			drawContours( greenBlobs, greenContours, i, color, 2, 8, gHierarchy, 0, Point() );
		}
		Mat disp;
		bitwise_or(greenBlobs, redBlobs, disp);

		/* show loaded frame */
		imshow(window_name, disp);

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
			return 1;
		}
		
    	}
	return 0;
}
