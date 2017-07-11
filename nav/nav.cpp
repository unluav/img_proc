#include "nav.hpp"
#include <cstdio>

using namespace std;
using namespace cv;

void sendToCtrl(Heading* head) {
	// TODO: INSERT NEAT FUNCTION TO SEND HEADING TO FLIGHT CONTROLLER
}

void focusClosestObject(Point2f* closest, Point2f* origin, vector<Point2f>* centers) {
	*closest = (*centers)[0];
	Point2f diff = (*centers)[0] - *origin;
	double dist = norm(diff), min_dist = dist;

	for (int i = 1; i < centers->size(); i++) {
		diff = (*centers)[i] - *origin;
		dist = norm(diff);

		if (dist < min_dist) {
			min_dist = dist;
			*closest = (*centers)[i];
		}
	}
}

void updateHeading(Heading* head, Prediction* pred, Point2f* origin) {
	Point2f diff = *origin - pred->point;
	double angle = atan2(diff.y, diff.x) / M_PI * 180;
	double weighted_dist = pred->confidence * norm(diff);

	head->magnitude = (int) (250 * weighted_dist / norm(*origin)) % 250;
	head->angle = (int) (360 + angle) % 360;
}
