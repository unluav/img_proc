#include "opencv2/core/utility.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/cudafilters.hpp"

#include <stdio.h>

using namespace cv;
using namespace std;

int edgeThresh = 1;
cuda::GpuMat gray, cuda_image, edge, cedge;
Mat image, init;

static void onTrackbar(int, void*){
  Ptr<cuda::Filter> filter = cuda::createBoxFilter(CV_8UC1, CV_8UC1, Size(3,3));

  filter->apply(gray, gray);
  
  Ptr<cv::cuda::CannyEdgeDetector> canny = cuda::createCannyEdgeDetector(edgeThresh, edgeThresh, 3);
  canny->detect(gray,gray);

  cuda_image.copyTo(cedge, gray);
  Mat e(cedge);
  imshow("Edge map", e);
}

static void help(){
      printf("\nThis sample demonstrates Canny edge detection\n"
           "Call:\n"
           "    /.edge [image_name -- Default is ../data/fruits.jpg]\n\n");

}

const char* keys = 
{
  "{help h||}{@image |../data/fruits.jpg|input image name}"
};

int main(int argc, const char** argv){
  CommandLineParser parser(argc, argv, keys);
  if(parser.has("help")){
    help();
    return 0;
 }

  string filename = parser.get<string>(0);

  image = imread(filename, 1);
  if(image.empty()){
    printf("Cannot read image file: %s\n", filename.c_str());
  }
  init.create(image.size(), image.type());
  cedge.upload(init);
  cuda_image.upload(image);
  cuda::cvtColor(cuda_image, gray, COLOR_BGR2GRAY);

  namedWindow("Edge map", 1);

  createTrackbar("Canny Threshold", "Edge Map", &edgeThresh, 100, onTrackbar);

  onTrackbar(0,0);
  waitKey(0);

  return(0);
}