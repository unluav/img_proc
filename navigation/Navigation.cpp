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
#include "../object_detection/detection_framework.hpp"
#include "./suggested_heading.hpp"
#include "../roomba_tracking/conf_arc.hpp"

using namespace std;
using namespace cv;

//  Retrieves the frame that is currently being outputted by the camera.
//  This will be the main method that needs to change when we swap over to a live feed
IplImage * query_image() {
    //TODO: Replace this w/ live feed
    static VideoPlayer * videoPlayer = cvCaptureFromFile(TEST_VIDEO_PATH);

    //Skip a number of frames based on the desired sampling frequency, loop video if necessary
    double fNum = cvGetCaptureProperty(video, CV_CAP_PROP_POS_FRAMES) + ( VID_FPS / QUERY_FREQUENCY ) ;
    if (fNum < 0)   //Hit end of video
        fNum = 0;
    cvSetCaptureProperty(video, CV_CAP_PROP_POS_FRAMES,fNum);
    
    return cvQueryFrame(video);
}

//DONE FOR NOW?
void * Navigation::update_heading() {
    bool __die = false;     //Local flag var I use to get around scope issues, may change later

    //Snag first two frames for ConfidenceArc instantiation (May be unecessary, but I wanted to)
    Point2f * first_frame_pts = query_image();
    this_thread::sleep_for (chrono::seconds(1.0 / QUERY_FREQUENCY));
    Point2f * second_frame_pts = query_image();
    ConfidenceArc conf_arc = new ConfidenceArc(first_frame_pts, second_frame_pts);


    while(!__die) {

        // Get current points
        Point2f * img_pts = IMAGINARY_LOGAN_FUNCTION_CALL_YAY(query_image());

        // Register points and get prediction
        conf_arc.predictNextFrame(img_pts);
        Prediction prediction = conf_arc.getPrediction();

        // Update heading
        heading_mtx.lock();
        sgtd_hdg.speed = SPEED_CALCULATION(prediction.confidence, DIST_FROM_ORG(prediction.point))
        sgtd_hdg.theta = THETA_CALCULATION(prediction.point)
        heading_mtx.unlock();

        die_mtx.lock();
        __die = die;
        die_mtx.unlock();
    }

    //CLEAN UP RESOURECES, RETURN
    return NULL;
}

//DONE
void * Navigation::die() {
    //Set kill flag and wait for the thread to spin down
    die_mtx.lock()
    die = true;
    die_mtx.unlock()

    t.join();
    alive = false;

    return NULL;
}

//DONE
void * Navigation::start() {
    //Spin up the thread and set property
    if (!alive) {
        t = thread(Navigation::update_heading)
        alive = true;
    } else {
        printf("A navigation thread is already running, please kill it before spawning a new one.\n");
    }

    return NULL;
}

//DONE
SuggestedHeading * Navigation::get_suggested_heading() {
    //Lock the mutex and make a deep copy of the suggested heading
    heading_mtx.lock()
    SuggestedHeading ret_val = { .theta = sgtd_hdg.theta, .speed = sgtd_hdg.speed }
    heading_mtx.unlock()    

    return &ret_val;
}