#include "nav.hpp"

using namespace std;
using namespace cv;

void sendToCtrl(Heading* heading) {
	// TODO: INSERT NEAT FUNCTION TO SEND HEADING TO FLIGHT CONTROLLER
}

void focusClosestObject(Point2f* closest_obj, Point2f* origin, vector<Point2f>* centers) {
	double dist = norm((*centers)[0] - *origin), min_dist = dist;
	*closest_obj = (*centers)[0];

	for (int i = 1; i < centers->size(); i++) {
		dist = norm((*centers)[i] - *origin);

		if (dist < min_dist) {
			min_dist = dist;
			*closest_obj = (*centers)[i];
		}
	}
}

void updateHeading(Heading* heading, Prediction* pred, Point2f* origin) {
	Point2f diff = *origin - pred->point;
	double angle = atan2(diff.y, diff.x) / M_PI * 180;
	double weighted_dist = pred->confidence * norm(diff);

	heading->magnitude = (int) (250 * weighted_dist / norm(*origin)) % 250;
	heading->angle = (int) (360 + angle) % 360;
}

void averageHeadings(Heading* heading, vector<Heading>* heading_record) {
	int size = heading_record->size();
	heading->magnitude = 0;
	heading->angle = 0;

	for (int i = 0; i < size; i++) {
		heading->magnitude += (*heading_record)[i].magnitude / size;
		heading->angle += (*heading_record)[i].angle / size;
	}
}

void reportAverageHeading(Heading* heading, vector<Heading>* heading_record) {
	averageHeadings(heading, heading_record);
	heading_record->clear();
	sendToCtrl(heading);
}
