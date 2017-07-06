#include "object_tracking/conf_arc.hpp"
#include "navigation/navigation.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Point2f* focusObject(Point2f* origin, Point2f centers[], int size) {
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

int main(int argc, char** argv) {
	Navigation * navigation = new Navigation();
	navigation->start();

	SuggestedHeading * hdg = new SuggestedHeading();
	navigation->get_suggested_heading( hdg );
	printf('Heading:\nTheta - %f\nSpeed - %f\n', hdg->theta, hdg->speed);

	navigation->die();

	return 0;
}
