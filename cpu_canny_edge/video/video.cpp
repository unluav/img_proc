#include <highgui.h>
#include <stdio.h>
#include <cv.h>
 
int main(int argc, char *argv[]) {
    int delay = 0, key=0, i=0;
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
    cvNamedWindow(window_name, CV_WINDOW_AUTOSIZE);
 
    /* Get the first frame and create a edges image with the same size */
    frame = cvQueryFrame(video);
    grey  = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
    edges = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
 
    /* calculate the delay between each frame and display video's FPS */
    printf("%2.2f FPS\n", cvGetCaptureProperty(video, CV_CAP_PROP_FPS));
    delay = (int) (1000/cvGetCaptureProperty(video, CV_CAP_PROP_FPS));
 
    while (frame) {
	/* Edges on the input gray image (needs to be grayscale) using the Canny algorithm.
           Uses two threshold and a aperture parameter for Sobel operator. */
        cvCvtColor(frame, grey, CV_BGR2GRAY);
        cvCanny( grey, edges, 1.0, 1.0, 3);
 
	/* show loaded frame */
        cvShowImage(window_name, edges);
 
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
