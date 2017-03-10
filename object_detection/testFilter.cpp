#include "filters.h"

using namespace cv; //  TODO: Stawp

	Mat TestFilter::processFrame(Mat& frame) {

		int threshold_value=50, threshold_type=2;
		int const max_BINARY_value = 255;

		Mat redBlobs1, redBlobs2, greenBlobs;

		//frm.MarkFrame();  //Daric's frame counter -- check gpu canny edge
		vector<vector<Point> > redContours; 		//vector is a list in c#
		vector<vector<Point> > greenContours;
		vector<Vec4i> rHierarchy;			//hierarchy?
		vector<Vec4i> gHierarchy;


		/* Edges on the input gray image (needs to be grayscale) using the Canny algorithm.
		Uses two threshold and a aperture parameter for Sobel operator. */
		cvtColor(frame, redBlobs1, CV_BGR2HSV);	//copies tempFrame into redBlobs with color for edge detection
		cvtColor(frame, redBlobs2, CV_BGR2HSV);	//color filters
		cvtColor(frame, greenBlobs, CV_BGR2HSV);
 
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

		return disp;
	}
