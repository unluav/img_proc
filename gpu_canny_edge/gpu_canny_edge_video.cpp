#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <cv.h>
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
    int delay = 0, key=0, i=0, counter=0;
    
    char *window_name;
    CvCapture *video = NULL;
    IplImage  *frame = NULL;
    IplImage  *grey  = NULL;
    IplImage  *edges = NULL;
 
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
    printf("%2.2f FPS\n", cvGetCaptureProperty(video, CV_CAP_PROP_FPS));
    delay = (int) (1000/cvGetCaptureProperty(video, CV_CAP_PROP_FPS));
    Ptr<cv::cuda::CannyEdgeDetector> canny = cuda::createCannyEdgeDetector(1, 100, 3);
    int last_time = time(0);
	while (frame) {
	cuda::GpuMat tempFrame, tempGrey, tempEdges;
	
	//Creating a Mat version of the current frame
	tempFrame.upload(cvarrToMat(frame));
	tempGrey.upload(cvarrToMat(grey));
	tempEdges.upload(cvarrToMat(edges));

	/* Edges on the input gray image (needs to be grayscale) using the Canny algorithm.
           Uses two threshold and a aperture parameter for Sobel operator. */
        cuda::cvtColor(tempFrame, tempGrey, CV_BGR2GRAY);
        canny->detect(tempGrey, tempEdges);
 	
	Mat e;
	tempEdges.download(e);
	/* show loaded frame */
       	imshow(window_name, e);
 	if(counter%50==0){
		printf("time:%ld\n", time(0)-last_time);
		last_time=time(0);
	}
	counter++;
	
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
