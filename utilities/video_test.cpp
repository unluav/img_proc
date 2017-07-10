#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include "FrameRateMonitor.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    string src = "nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)I420, framerate=(fraction)30/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    if (argc >= 2){
        src = argv[1];
    }
    cout << "Choosing feed ['" << src << "']\n"; 
    VideoCapture cap(src);
    cout << "Initializing Feed\n";
    if (!cap.isOpened()){
        cout << "Failed to open feed\n";
        return -1;
    }
    namedWindow("edges", 1);
    FrameRateMonitor frm;
    
    frm.Start();
    int count = 0;
    for (;;)
    {
	frm.MarkFrame();
        Mat frame;
        cap >> frame;
        printf("Processing frame %d\n", count);
        imshow("edges", frame);
        count++;
        if (waitKey(1) >= 0) break;
    }
    frm.Stop();
    frm.DumpInfo();
    return 0;
}
