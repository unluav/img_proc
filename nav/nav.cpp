#include "nav.hpp"
#include <cstdio>

using namespace std;
using namespace cv;

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

void updateHeading(Heading* hdg, ConfidenceArc* arc) {
	Point2f diff = *arc->getCurrent() - arc->getPrediction()->point;
	int angle = (int) (atan2(diff.y, diff.x) / M_PI * 180);

	hdg->magnitude = (int) (arc->getPrediction()->confidence * 250);
	hdg->angle = angle > 0 ? angle : angle + 360;
}
