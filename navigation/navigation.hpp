#ifndef NAVIGATION
#define NAVIGATION

typedef struct suggested_heading
{
	double theta;
	double speed;
} SuggestedHeading;

class Navigation {

private:
	SuggestedHeading 	sgtd_hdg 			= 	{.theta = 0, .speed = 0};
	bool				die 				= 	false;
	bool				alive				=	false;
	std::thread 		t					=	NULL
	static std::mutex 	die_mtx;
	static std::mutex 	heading_mtx;

	void * 				update_heading();

public:
	void *				die();
	void *				start();
	SuggestedHeading *	get_suggested_heading();

} nav;

#endif
