#include "nav.hpp"

using namespace std;
using namespace cv;

void sendToCtrl(Heading* head) {
	// TODO: INSERT NEAT FUNCTION TO SEND HEADING TO FLIGHT CONTROLLER
}

void focusClosestObject(Point2f* closest, Point2f* origin, vector<Point2f>* centers) {
	double dist = norm((*centers)[0] - *origin), min_dist = dist;
	*closest = (*centers)[0];

	for (int i = 1; i < centers->size(); i++) {
		dist = norm((*centers)[i] - *origin);

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

void averageHeadings(Heading* head, vector<Heading>* heads) {
	int size = heads->size();
	head->magnitude = 0;
	head->angle = 0;

	for (int i = 0; i < size; i++) {
		head->magnitude += (*heads)[i].magnitude / size;
		head->angle += (*heads)[i].angle / size;
	}
}
