using namespace std;
#include <stdio.h>
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
    
    Mat tempFrame, redBlobs1, redBlobs2,greenBlobs;
    int frmCounter = 0;
    while (frame) {
	frmCounter++;
	if(frmCounter%3 == 0){
		continue;
	}
        //frm.MarkFrame();  
	//vector<vector<Point> > redContours; 
	vector<vector<Point> > greenContours;
	vector<Vec4i> rHierarchy;
	vector<Vec4i> gHierarchy;
	
	//Creating a Mat version of the current frame
	cvarrToMat(frame).copyTo(tempFrame);
	//cvarrToMat(blobs).copyTo(redBlobs1);
	//cvarrToMat(blobs).copyTo(redBlobs2);
	cvarrToMat(blobs).copyTo(greenBlobs);
	

	/* Edges on the input gray image (needs to be grayscale) using the Canny algorithm.
           Uses two threshold and a aperture parameter for Sobel operator. */
    //cvtColor(tempFrame, redBlobs1, CV_BGR2HSV);
    //cvtColor(tempFrame, redBlobs2, CV_BGR2HSV);
 	cvtColor(tempFrame, greenBlobs, CV_BGR2HSV);
 
	/*Finding the color blobs (needs to be a colored image) using Inrange*/
	//inRange(redBlobs1, Scalar(160,80,80), Scalar(179,255,255),redBlobs1);
	//inRange(redBlobs2, Scalar(160,80,80), Scalar(179,255,255),redBlobs2);
	//Mat redBlobs;
	//bitwise_or(redBlobs1, redBlobs2, redBlobs);
	inRange(greenBlobs, Scalar(40,80,80), Scalar(80,255,255), greenBlobs);
    // 	findContours(redBlobs, redContours, rHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
 	findContours(greenBlobs, greenContours, gHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
 	
	vector<Moments> mu(greenContours.size());
 	for( int i = 0; i < greenContours.size(); i++ )
    	{ mu[i] = moments( greenContours[i], false ); }

    //  Get the mass centers:
  	vector<Point2f> mc( greenContours.size() );
   
  	for( int i = 0; i < greenContours.size(); i++ )
   	{ mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
    
   	for( int i = 0; i< greenContours.size(); i++ )
   	{
       Scalar color = Scalar( 60,255,255 );
      drawContours( greenBlobs, greenContours, i, color, 2, 8, gHierarchy, 0, Point() );
      circle( greenBlobs, mc[i], 4, color, -1, 8, 0 );
   	}
 	Mat disp;
 	//bitwise_or(greenBlobs, redBlobs, disp);

	/* show loaded frame */
       	imshow(window_name, greenBlobs);
	
	/* load and check next frame*/
        frame = cvQueryFrame(video);
	if(!frame) {
	    printf("error loading frame.\n");
		return 1;
	}

	/* wait delay and check for the quit key */
        key = cvWaitKey(delay);
        if(key=='q') break;
    }
}
