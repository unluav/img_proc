#include "nav.hpp"

Point2f focusObject(Point2f* position, vector<Point2f>* centers) {
	Point2f closest = (*centers)[0], diff = (*centers)[0] - *position;
	double dist = norm(diff), min_dist = dist;

	for (int i = 1; i < centers->size(); i++) {
		diff = (*centers)[i] - *position;
		dist = norm(diff);

		if (dist < min_dist) {
			min_dist = dist;
			closest = (*centers)[i];
		}
	}

	return closest;
}

Heading suggestHeading(ConfidenceArc* arc) {
	Heading hdg;
	hdg.magnitude = (uint16_t) arc->getPrediction()->confidence * 250;
	Point2f diff = *arc->getCurrent() - arc->getPrediction()->point;
	hdg.angle = (uint16_t) atan2(diff.y, diff.x) * 359 / M_PI;
	return hdg;
}
