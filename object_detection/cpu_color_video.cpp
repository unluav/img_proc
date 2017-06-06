using namespace std;
#include <stdio.h>
#include <map>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
int CenterTracking(Point2f Centers[],IplImage *frame ) {
    int delay = 0, key=0, i=0, counter=0;

/*calculate size of array*/
  int size = sizeof(Centers)/ sizeof(Point2f);

//    char *window_name;
//    CvCapture *video = NULL;
//    IplImage  *frame = NULL;
    //IplImage  *grey  = NULL;
    IplImage  *blobs = NULL;
//	int threshold_value=50, threshold_type=2;
//	int const max_BINARY_value = 255;
   //Point2f Com = Point2f (0,0);

    /* check for video file passed by command line 
    if (argc>1) {
        video = cvCaptureFromFile(argv[1]);
    } else {
        printf("Usage: %s VIDEO_FILE\n", argv[0]);
        return 1;
    }*/
 
    /* check frame was correctly loaded */
    if (blobs==NULL) {
        printf("error");
        return -1;
    }
 
    /* create a video window with same name of the video file, auto sized */
//    window_name = argv[1];
//    namedWindow(window_name, 1);
 
    /* Get the first frame and create a edges image with the same size */
//    frame = cvQueryFrame(video);
    //grey  = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
    blobs = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);

    /* calculate the delay between each frame and display video's FPS */
   // delay = (int) (1000/cvGetCaptureProperty(video, CV_CAP_PROP_FPS));
    
    Mat tempFrame, redBlobs1, redBlobs2,greenBlobs;
//    int frmCounter = 0;
//    while (frame) {
//	frmCounter++;
//	if(frmCounter%3 == 0){
//		continue;
//	}
        //frm.MarkFrame();  
	vector<vector<Point> > redContours; 
	vector<vector<Point> > greenContours;
	vector<Vec4i> rHierarchy;
	vector<Vec4i> gHierarchy;
	

	int centerWidth = blobs->width/2;
	int centerHeight = blobs->height/2;
	//Creating a Mat version of the current frame
	cvarrToMat(frame).copyTo(tempFrame);
	cvarrToMat(blobs).copyTo(redBlobs1);
	cvarrToMat(blobs).copyTo(redBlobs2);
	cvarrToMat(blobs).copyTo(greenBlobs);
	blur(tempFrame, tempFrame, Size(3,3) );	

	/* Edges on the input gray image (needs to be grayscale) using the Canny algorithm.
           Uses two threshold and a aperture parameter for Sobel operator. */
    cvtColor(tempFrame, redBlobs1, CV_BGR2HSV);
    cvtColor(tempFrame, redBlobs2, CV_BGR2HSV);
 	cvtColor(tempFrame, greenBlobs, CV_BGR2HSV);
 
	/*Finding the color blobs (needs to be a colored image) using Inrange*/
	inRange(redBlobs1, Scalar(0,150,150), Scalar(25,250,250),redBlobs1);
	inRange(redBlobs2, Scalar(155,70,70), Scalar(180,200,200),redBlobs2);
	Mat redBlobs;
	bitwise_or(redBlobs1, redBlobs2, redBlobs);
	inRange(greenBlobs, Scalar(50,80,80), Scalar(130,255,255), greenBlobs);
 	findContours(redBlobs, redContours, rHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
 	findContours(greenBlobs, greenContours, gHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
 	
// 	for( int i = 0; i< redContours.size(); i++ )
  //  {
    //  Scalar color = Scalar( 0,255,255);
     // drawContours( redBlobs, redContours, i, color, 2, 8, rHierarchy, 0, Point() );
   // }
   
     // drawContours( greenBlobs, greenContours, i, color, 2, 8, gHierarchy, 0, Point() );


/*calculate min enclosing circles for every red and green contour*/

vector<vector<Point> > contours_poly(greenContours.size());
vector<vector<Point> > contours_poly2(redContours.size());
vector<Point2f>center(greenContours.size());
vector<Point2f>center2(redContours.size());
vector<float>radius(greenContours.size());
vector<float>radius2(redContours.size());
for(int i = 0;i<redContours.size();i++){
	approxPolyDP( Mat(redContours[i]),contours_poly2[i],3,true);
	minEnclosingCircle( (Mat)contours_poly2[i],center2[i],radius2[i]);
}
for(int i = 0;i<greenContours.size();i++){
	approxPolyDP( Mat(greenContours[i]),contours_poly[i],3,true);
	minEnclosingCircle( (Mat)contours_poly[i],center[i],radius[i]);
}

//Mat drawing = Mat::zeros( greenBlobs.size(),CV_8UC3);
//Mat drawing2 = Mat::zeros( greenBlobs.size(),CV_8UC3);

float GreenMaxRadius[5]={};
float RedMaxRadius[5]={};
int GreenMax[5]={};
int RedMax[5]={};
Point2f GreenCenters[5];
Point2f RedCenters[5];
//Point2f Centers[10];
int flag=0;
int num=0;
int num2=0;
//Point2f TempGreenCenters[10]=GreenCenters;
//Point2f TempRedCenters[10]=RedCenters;
//Point2f TempGreenCenters2[10];
//Point2f TempRedCenters2[10];
//int TempGreenRadius[10]=GreenMaxRadius;
//int TempRedRadius[10]=RedMaxRadius;

//TempCom1=center2[RedMax[0]];

/*find five largest red and green objects with radius above 20 pixles to reduce noise*/

for(int i=0;i<redContours.size();i++){
	flag=0;
	for(int j=0;j<5;j++){
		if(radius2[i]>RedMaxRadius[j] && flag==0 && radius2[i]>20){
			RedMaxRadius[j]=radius2[i];
			RedMax[j]=i;
			RedCenters[j]=center2[i];
			flag=1;
			num++;
		}
	}
}
for(int i=0;i<greenContours.size();i++){
	flag=0;
	for(int j=0;j<5;j++){
		if(radius[i]>GreenMaxRadius[j] && flag==0 && radius[i]>20){
			GreenMaxRadius[j]=radius[i];
			GreenCenters[j]=center[i];
			GreenMax[j]=i;
			flag=1;
			num2++;
		}
	}
}

/*combine red and green arrays into one array and fill rest of array with -1,-1*/

for(int i=0;i<size;i++){
	if(i<num){
		Centers[i]=RedCenters[i];
	}else if(i<num+num2){
		Centers[i]=GreenCenters[i-num];
	}else{
		Centers[i]= Point2f (-1,-1);
	}
}

//TempGreenCenters2=GreenCenters;
//TempGreenCenters2=RedCenters;
/*
if(first!=0){
	for(int i=0;i<10;i++){
		if(sqrt(pow(RedCenters[i].x-TempRedCenters[i].x,2)+pow(RedCenters[i].y-TempRedCenters[i].y,2))>=TempRedRadius[i]){
			for(j=0;j<10,j++){
				if(sqrt(pow(RedCenters[j].x-TempRedCenters[j].x,2)+pow(RedCenters[j].y-TempRedCenters[j].y,2))>=RedMaxRadius[j]){
					
				}
			}
		}
	}
}

*/
/*
Scalar color=Scalar (0,0,255);
for(int i=0; i<redContours.size();i++){
	drawContours(drawing2,contours_poly2,i,color,1,8,vector<Vec4i>(),0,Point());
	for(int j=0;j<10;j++){
		if(radius2[i]==RedMaxRadius[j]){
			circle(drawing2,center2[i],(int)radius2[i],color,2,8,0);
		}
	}
}

color=Scalar (0,255,0);
for(int i=0; i<greenContours.size();i++){
	drawContours(drawing,contours_poly,i,color,1,8,vector<Vec4i>(),0,Point());
	for(int j=0;j<10;j++){
		if(radius[i]==GreenMaxRadius[j]){
			circle(drawing,center[i],(int)radius[i],color,2,8,0);
		}
	}
}
*/
// 	Mat disp;
// 	bitwise_or(drawing, drawing2, disp);
	
	/* show loaded frame */
//       	imshow(window_name, disp);
	
	/* load and check next frame*/
//        frame = cvQueryFrame(video);
//	if(!frame) {
//	    printf("error loading frame.\n");
//		return 1;
//	}
 
	/* wait delay and check for the quit key */
//        key = cvWaitKey(delay);
//        if(key=='q') break;
//    }

/*return number of detected objects*/

return num+num2;

}
