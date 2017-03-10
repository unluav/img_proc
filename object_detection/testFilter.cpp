#include "filters.h"

	cv::Mat TestFilter::processFrame(cv::Mat& frame) {

		int threshold_value=50, threshold_type=2;
		int const max_BINARY_value = 255;

		cv::Mat redBlobs1, redBlobs2, greenBlobs;

		//frm.MarkFrame();  //Daric's frame counter -- check gpu canny edge
		cv::vector<cv::vector<cv::Point> > redContours; 		//vector is a list in c#
		cv::vector<cv::vector<cv::Point> > greenContours;
		cv::vector<cv::Vec4i> rHierarchy;			//hierarchy?
		cv::vector<cv::Vec4i> gHierarchy;


		/* Edges on the input gray image (needs to be grayscale) using the Canny algorithm.
		Uses two threshold and a aperture parameter for Sobel operator. */
		cv::cvtColor(frame, redBlobs1, CV_BGR2HSV);	//copies tempFrame into redBlobs with color for edge detection
		cv::cvtColor(frame, redBlobs2, CV_BGR2HSV);	//color filters
		cv::cvtColor(frame, greenBlobs, CV_BGR2HSV);
 
		/*Finding the color blobs (needs to be a colored image) using Inrange*/
		cv::inRange(redBlobs1, cv::Scalar(160,80,80), cv::Scalar(179,255,255),redBlobs1);
		cv::inRange(redBlobs2, cv::Scalar(0,80,80), cv::Scalar(10,255,255),redBlobs2);

		cv::Mat redBlobs;
		cv::bitwise_or(redBlobs1, redBlobs2, redBlobs);
		cv::inRange(greenBlobs, cv::Scalar(40,80,80), cv::Scalar(80,255,255), greenBlobs);
		// 	findContours(redBlobs, redContours, rHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		cv::findContours(greenBlobs, greenContours, gHierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

		// 	for( int i = 0; i< redContours.size(); i++ )
		//   {
		//     Scalar color = Scalar( 0,255,255);
		//     drawContours( redBlobs, redContours, i, color, 2, 8, rHierarchy, 0, Point() );
		//   }
		for( int i = 0; i< greenContours.size(); i++ )
		{
			cv::Scalar color = cv::Scalar( 120,255,255 );
			//instead of using i, we could use -1, which would draw them all
			cv::drawContours( greenBlobs, greenContours, i, color, 2, 8, gHierarchy, 0, cv::Point() );
		}
		cv::Mat disp;
		cv::bitwise_or(greenBlobs, redBlobs, disp);

		return disp;
	}
