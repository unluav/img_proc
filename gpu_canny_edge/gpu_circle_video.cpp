using namespace std;
#include <stdio.h>
//#include "../utilities/FrameRateMonitor.h"
#include "opencv2/gpu/gpu.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
int main(int argc, char *argv[]) {
    int delay = 0, key=0, i=0, counter=0;
    
    char *window_name;
    CvCapture *video = NULL;
    IplImage  *frame = NULL;
    IplImage  *grey  = NULL;
    IplImage  *edges = NULL;
	int threshold_value=15, threshold_type=3;
	int const max_value = 255;
	int const max_type = 3;
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
    grey  = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
    edges = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
 
    /* calculate the delay between each frame and display video's FPS */
    delay = (int) (1000/cvGetCaptureProperty(video, CV_CAP_PROP_FPS));

    int frmCounter = 0;
    //FrameRateMonitor frm;
    //frm.Start();
    while (frame) {
	if(frmCounter%4 == 0){
		continue;
	}
        //frm.MarkFrame();  
	vector<Vec3f> circles;  
	gpu::GpuMat tempFrame, tempGrey, tempEdges;
	
	//Creating a Mat version of the current frame
	tempFrame.upload(cvarrToMat(frame));
	tempGrey.upload(cvarrToMat(grey));
	tempEdges.upload(cvarrToMat(edges));

	/* Edges on the input gray image (needs to be grayscale) using the Canny algorithm.
           Uses two threshold and a aperture parameter for Sobel operator. */
    gpu::cvtColor(tempFrame, tempGrey, CV_BGR2GRAY);
	GaussianBlur( tempGrey, tempGrey, Size(9,9), 2, 2 );
	threshold(tempGrey, tempEdges, threshold_value, max_BINARY_value,threshold_type);
       
 	
	Mat e;
	tempEdges.download(e);
	HoughCircles( e, circles, CV_HOUGH_GRADIENT, 1, tempGrey.rows/8, 200, 100, 0, 0 );
	
  for( int i = 0; i< circles.size(); i++ )
     {
		      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
      circle( e, center, 3, Scalar(255,0,0,255), -1, 8, 0 );
      // circle outline
      circle( e, center, radius, Scalar(255,0,0,255), 3, 8, 0 );
     }
	/* show loaded frame */
       	imshow(window_name, e);
	
	/* load and check next frame*/
        frame = cvQueryFrame(video);
		frmCounter+=1;
	if(!frame) {
          //  frm.Stop();
          // frm.DumpInfo();
	    printf("error loading frame.\n");
		return 1;
	}
 
	/* wait delay and check for the quit key */
        key = cvWaitKey(delay);
        if(key=='q') break;
    }
    //frm.Stop();
    //frm.DumpInfo();
}
