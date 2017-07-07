#include "object_tracking/conf_arc.hpp"
#include "navigation/navigation.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	Navigation * navigation = new Navigation();
	navigation->start();

	SuggestedHeading * hdg = new SuggestedHeading();

	int count = 1;
	while(true) {
		navigation->get_suggested_heading( hdg );
		printf("Frame: %d\tHeading:\nTheta - %f\nSpeed - %f\n\n", count++, hdg->theta, hdg->speed);
		this_thread::sleep_for(chrono::milliseconds(100));
	}

	navigation->die();

	return 0;
}
