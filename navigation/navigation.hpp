#ifndef NAVIGATION
#define NAVIGATION

#define     QUERY_FREQUENCY                 10                      //  Number of frames processed per second
#define     MAX_SPEED_DISTANCE              100                     //  Defines the distance at which the suggested heading speed plateaus
#define     TEST_VIDEO_PATH                 "/uav_rsc/2roomba.mp4"  //  NOTE: This will eventually go away when we implement live feed
#define     VID_FPS                         30                      //  NOTE: This will also go away
#define     OBJECT_COUNT                    5                       //  Number of roombas expected

#define     MAX(a,b)                        ( ( (a) > (b) ) ? (a) : (b) )
#define     DIST_FROM_ORG(pt)               ( sqrt( pow( (pt).x, 2 ) + pow( (pt).y, 2 ) ) )
#define     SPEED_CALCULATION(conf,dist)    ((double) MAX(MAX_SPEED_DISTANCE, dist) / (double) MAX_SPEED_DISTANCE) * conf  //May need to change this, I can't remember what we agreed upon...
#define     THETA_CALCULATION(pt)           ( tan( (double)(pt).x / (double)(pt).y ) )

#include    <thread>
#include    <mutex>

typedef struct suggested_heading
{
    double theta;
    double speed;
} SuggestedHeading;

class Navigation {
private:
    SuggestedHeading    _sgtd_hdg           =   {.theta = 0, .speed = 0};
    bool                _die                =   false;
    bool                _alive              =   false;
    std::thread *       _t;
    std::mutex *        _die_mtx;
    std::mutex *        _heading_mtx;

    void *              _update_heading();

public:
    void *              die();
    void *              start();
    SuggestedHeading *  get_suggested_heading( SuggestedHeading * sgst_hdg_buf );

};

#endif
