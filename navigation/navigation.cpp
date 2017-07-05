/*======================================================================================================*
 *  navigation.cpp   Author: Jared Frenzel  Date: June 8, 2017                                          *
 * ---------------------------------------------------------------------------------------------------- *   
 *  This unit of code aims to utilize the functionality of roomba tracking and object detection to      *
 *  deliver navigation instructions to the UAV in the form of heading suggestions accessed with the     *
 *  get_suggested_heading method. To start navigation, create a Navigation object and call the run      *
 *  method. Note that this is a singleton object. When finished, call the die method to end the thread. *
 *======================================================================================================*/

#include "navigation.hpp"

#include <iostream>
#include <iomanip>
#include <map>
#include <stdio.h>
#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "../object_detection/cpu_color_video.hpp"
#include "../object_tracking/conf_arc.hpp"

#include "../object_detection/video_player/videoPlayer.hpp"


using namespace std;
using namespace cv;

//  Retrieves the frame that is currently being outputted by the camera.
//  This will be the main method that needs to change when we swap over to a live feed
IplImage * query_image() {
    //TODO: Replace this w/ live feed
    CvCapture * video = cvCaptureFromFile( (TEST_VIDEO_PATH) );

    //Skip a number of frames based on the desired sampling frequency, loop video if necessary
    double fNum = cvGetCaptureProperty(video, CV_CAP_PROP_POS_FRAMES) + ( VID_FPS / QUERY_FREQUENCY ) ;
    if (fNum < 0)   //Hit end of video
        fNum = 0;
    cvSetCaptureProperty(video, CV_CAP_PROP_POS_FRAMES,fNum);
    
    return cvQueryFrame(video);
}


Point2f* focusObject(Point2f* origin, Point2f centers[], int size) {
    /*
     * This method takes an array of centers an returns the point that is closest to the 
     * given point (typically the center of the screen) 
     * AUTHOR: Jacob Koperski
     */

    int closest = 0;
    Point2f diff(0, 0);
    double min_dist = norm(diff);

    for (int i = 0; i < size; i++) {
        diff = Point2f(centers[i].x - origin->x, centers[i].y - origin->y);
        double dist = norm(diff);

        if (dist < min_dist) {
            min_dist = dist;
            closest = i;
        }
    }

    return &centers[closest];
}

//DONE FOR NOW?
void * Navigation::_update_heading() {
    bool __die = false;     //Local flag var I use to get around scope issues, may change later

    //Snag first two frames for ConfidenceArc instantiation (May be unecessary, but I wanted to)
    Point2f first_frame_pts[25];
    Point2f second_frame_pts[25];

    IplImage * first_frame      =   query_image();
    int first_frame_size        =   CenterTracking(first_frame_pts, first_frame);

    this_thread::sleep_for(chrono::milliseconds(1000 / (QUERY_FREQUENCY)));

    IplImage * second_frame     =   query_image();
    int second_frame_size       =   CenterTracking(second_frame_pts, second_frame);

    ConfidenceArc * conf_arc    =   new ConfidenceArc(first_frame_pts, second_frame_pts);
    Prediction * prediction     =   conf_arc->getPrediction();


    int size = 0;
    Point2f centers[25];
    Point2f * focused = NULL;
    Point2f origin((double) first_frame->width / 2, (double) first_frame->height / 2);


    while(!__die) {

        // Get current closest point
        size = CenterTracking(centers, query_image());
        focused = focusObject(&origin, centers, size);

        // Register points and get next prediction
        conf_arc->predictNextFrame(focused);

        // Update heading
        _heading_mtx.lock();
        _sgtd_hdg.speed = SPEED_CALCULATION(prediction->confidence, DIST_FROM_ORG(prediction->point));
        _sgtd_hdg.theta = THETA_CALCULATION(prediction->point);
        _heading_mtx.unlock();

        _die_mtx.lock();
        __die = _die;
        _die_mtx.unlock();

        this_thread::sleep_for (chrono::milliseconds(1000 / (QUERY_FREQUENCY)));
    }

    //CLEAN UP RESOURECES, RETURN
    return NULL;
}

//DONE
void * Navigation::die() {
    //Set kill flag and wait for the thread to spin down
    _die_mtx.lock();
    _die = true;
    _die_mtx.unlock();

    _t.join();
    _alive = false;

    return NULL;
}

//DONE
void * Navigation::start() {
    //Spin up the thread and set property
    if (!_alive) {
        _t = std::thread{Navigation::_update_heading};
        _alive = true;
    } else {
        printf("A navigation thread is already running, please kill it before spawning a new one.\n");
    }

    return NULL;
}

//DONE
SuggestedHeading * Navigation::get_suggested_heading() {
    //Lock the mutex and make a deep copy of the suggested heading
    _heading_mtx.lock();
    SuggestedHeading ret_val = new SuggestedHeading { .theta = _sgtd_hdg.theta, .speed = _sgtd_hdg.speed };
    _heading_mtx.unlock();

    return &ret_val;
}