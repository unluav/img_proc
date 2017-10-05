#ifndef GRID_DETECT_HPP
#define GRID_DETECT_HPP

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/opencv.hpp>

void detectLines(cv::Mat* frame, cv::VideoWriter outputVideo, int max_obj_count = 5);

#endif
