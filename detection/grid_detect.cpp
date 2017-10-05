#include "grid_detect.hpp"
#include <cstdio>

using namespace std;
using namespace cv;

/*bool BY_RADIUS(Circle first, Circle second) {
	return first.radius <= second.radius;
}

void findBoundingCircles(vector<vector<Point>>* contours, vector<Circle>* circ) { 
	int size = contours->size(), min_radius = 20;
	vector<vector<Point>> polygons(size);
	Circle c;

	for (int i = 0; i < size; i++) {
		approxPolyDP(Mat((*contours)[i]), polygons[i], 3, true);
		minEnclosingCircle((Mat) polygons[i], c.center, c.radius);

		if (c.radius > min_radius) {
			circ->push_back(c);
		}
	}
}

void filterLargest(vector<Circle>* key_circ, vector<Circle>* circ, int max_obj_count, Mat* frame, Scalar color) {
	int size = circ->size(), circ_count = min(size, max_obj_count), j;
	sort(circ->begin(), circ->end(), BY_RADIUS);

	for (int i = 1; i <= circ_count; i++) {
		j = size - i;
		key_circ->push_back((*circ)[j]);
		circle(*frame, (*circ)[j].center, (*circ)[j].radius, color, 3);
		circle(*frame, (*circ)[j].center, 2, color, 3);
	}
}*/

void detectLines(Mat* frame, VideoWriter outputVideo, int max_obj_count) {
        // Blur the image to remove some of the noise
        // Output will be put in the 'blurred' matrix
        // Change the Size values to change how much is blurred out
        // Higher numbers will blur more (fewer lines and less noise will be detected)
        // Lower numbers will blur less (more lines and more noise will be detected)
        // See GaussianBlur on http://docs.opencv.org/2.4/doc/tutorials/imgproc/gausian_median_blur_bilateral_filter/gausian_median_blur_bilateral_filter.html
        Mat blurred;
        GaussianBlur( *frame, blurred, Size( 15, 15 ), 0, 0 );

        // Run Canny edge detection to find edges in the 'blurred' matrix image
        // Output will be put in the 'edges' matrix
        // See http://docs.opencv.org/2.4/modules/imgproc/doc/feature_detection.html?highlight=canny
        Mat edges;
        Canny(blurred, edges, 0, 100, 3, 0);
        
        // Run HoughLines to detect straight lines in the image
        Mat result;
        // Convert color of image in 'result'
        cvtColor(edges, result, CV_GRAY2BGR);
        vector<Vec4i> lines;
        // Get the lines using the HoughLines function
        // 'edges' is input, 'lines' is output
        // See http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/hough_lines/hough_lines.html
        HoughLinesP(edges, lines, 1, CV_PI/180, 50, 50, 10 );
        // Draw the lines that were found onto the 'result' matrix
        for( size_t i = 0; i < lines.size(); i++ )
        {
                Vec4i l = lines[i];
                line( result, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
        }

        outputVideo << result;
        // Display the end result on the screen
        imshow("detected lines", result);
}



	//cuda::GpuMat d_frame;
	//d_frame.upload(*frame);

//, d_red_blobs, d_grn_blobs;
	//Mat h_red_blobs, h_grn_blobs;
	//vector<vector<Point>> red_contours, grn_contours;
	//vector<Circle> red_circ, grn_circ, key_circ;



        /* CV_Assert(frame->depth() == CV_8U);  // accept only uchar images
        Mat Result;

        Result.create(frame->size(), frame->type());
        /*const int nChannels = frame->channels();

        for(int j = 1; j < frame->rows - 1; ++j)
        {

                uchar* output = Result.ptr<uchar>(j);

                for(int i = nChannels; i < nChannels * (frame->cols - 1); ++i)
                {
                        *output = 0;
                }
        }

        Result.row(0).setTo(Scalar(0));
        Result.row(Result.rows - 1).setTo(Scalar(0));
        Result.col(0).setTo(Scalar(0));
        Result.col(Result.cols - 1).setTo(Scalar(0));

        

        d_frame.upload(Result);
        imshow("detected lines", Result);*/

        //imshow("testwindow", test2);
           

	//cuda::cvtColor(d_frame, d_frame, COLOR_RGB2HSV);

	//d_red_blobs = cuda::GpuMat(d_frame.rows, d_frame.cols, CV_8UC1);
	//d_grn_blobs = cuda::GpuMat(d_frame.rows, d_frame.cols, CV_8UC1);

	//cudaInRange(d_frame, Scalar(110, 0, 0), Scalar(149, 215, 215), d_red_blobs);
	//cudaInRange(d_frame, Scalar(0, 80, 80), Scalar(49, 255, 255), d_grn_blobs);

	//d_red_blobs.download(h_red_blobs);
	//d_grn_blobs.download(h_grn_blobs);

	//findContours(h_red_blobs, red_contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	//findContours(h_grn_blobs, grn_contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	//findBoundingCircles(&red_contours, &red_circ);
	//findBoundingCircles(&grn_contours, &grn_circ);

	//filterLargest(&key_circ, &red_circ, max_obj_count, frame, Scalar(0, 0, 255));
	//filterLargest(&key_circ, &grn_circ, max_obj_count, frame, Scalar(0, 255, 0));

	/*centers->clear();
	for (int i = 0; i < key_circ.size(); i++) {
		centers->push_back(key_circ[i].center);
	}*/
//}
