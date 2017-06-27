#ifndef NAVIGATION
#define NAVIGATION

#define     QUERY_FREQUENCY                 10                  //  Number of frames processed per second
#define     MAX_SPEED_DISTANCE              100                 //  Defines the distance at which the suggested heading speed plateaus 
#define     TEST_VIDEO_PATH                 "change.me"         //  NOTE: This will eventually go away when we implement live feed
#define     VID_FPS                         30                  //  NOTE: This will also go away

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
    SuggestedHeading    sgtd_hdg            =   {.theta = 0, .speed = 0};
    bool                die                 =   false;
    bool                alive               =   false;
    std::thread         t;
    static std::mutex   die_mtx;
    static std::mutex   heading_mtx;

    void *              update_heading();

public:
    void *              die();
    void *              start();
    SuggestedHeading *  get_suggested_heading();

} nav;

#endif
