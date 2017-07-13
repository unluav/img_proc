#include "conf_arc.hpp"

using namespace std;
using namespace cv;

Prediction::Prediction(Point2f pt = Point2f(0.0f, 0.0f), double conf = 0, double rad = 0, double rng = DEF_RANGE) {
	this->point = pt;
	this->confidence = conf;
	this->radius = rad;
	this->range = rng;
}

void Prediction::setRange(double rng) {
	this->range = rng;
}

ConfidenceArc::ConfidenceArc() {
	Point2f prev(0.0f, 0.0f), curr(0.0f, 0.0f);
	int num = DEF_BACKTRACE;
	this->path.push_back(prev);
	this->path.push_back(curr);
	this->prev = prev;
	this->curr = curr;
	this->backtrace = num;

	this->prediction = Prediction();
	this->recordError(curr, prev);
	this->recordError(curr, prev);
	this->predict();
}

ConfidenceArc::ConfidenceArc(Point2f prev = Point2f(0.0f, 0.0f), Point2f curr = Point2f(0.0f, 0.0f), int num = DEF_BACKTRACE) {
	this->path.push_back(prev);
	this->path.push_back(curr);
	this->prev = prev;
	this->curr = curr;
	this->backtrace = num;

	this->prediction = Prediction();
	this->recordError(curr, prev);
	this->recordError(curr, prev);
	this->predict();
}

vector<Point2f>* ConfidenceArc::getPath() {
	return &this->path;
}

Prediction* ConfidenceArc::getPrediction() {
	return &this->prediction;
}

vector<double>* ConfidenceArc::getErrors() {
	return &this->errors;
}

Point2f* ConfidenceArc::getPrevious() {
	return &this->prev;
}

Point2f* ConfidenceArc::getCurrent() {
	return &this->curr;
}

int* ConfidenceArc::getBacktrace() {
	return &this->backtrace;
}

void ConfidenceArc::setBacktrace(int num) {
	this->backtrace = num;
}

void ConfidenceArc::predictNextFrame(vector<Point2f>* centers, vector<ConfidenceArc>* arcs) {
	for (int i = 0; i < centers->size(); i++) {
		(*arcs)[i].predictNextFrame(&(*centers)[i]);
	}
}

void ConfidenceArc::predictNextFrame(Point2f* current) {
	this->path.push_back(*current);
	this->prev = this->curr;
	this->curr = *current;
	this->recordError(this->prediction.point, this->curr);
	this->predict();
}

void ConfidenceArc::recordError(Point2f p1, Point2f p2) {
	double dist = norm(p1 - p2), threshold = this->prediction.radius * this->prediction.range;
	this->errors.push_back(dist < threshold ? 0 : dist);
}

void ConfidenceArc::predict() {
	this->prediction.point = this->predictPoint();
	this->prediction.radius = this->predictRadius();
	this->prediction.confidence = this->predictConfidence();
}

// Calculates point of a prediction via linear approximation
Point2f ConfidenceArc::predictPoint() {
	return 2 * this->curr - this->prev;
}

// Calculates radius of a prediction by sampling the errors from previous predictions
double ConfidenceArc::predictRadius() {
	double mean = 0, variance = 0;
	int size = this->errors.size(), length = this->backtrace <= 0 ? size : min(this->backtrace, size);

	for (int i = 1; i <= length; i++) {
		mean += this->errors[size - i] / length;
	}

	for (int i = 1; i <= length; i++) {
		variance += pow(this->errors[size - i] - mean, 2);
	}

	pair<double, double> stats = make_pair(mean, sqrt(variance));
	default_random_engine sample;
	normal_distribution<double> distribution(stats.first, stats.second);
	return fabs(distribution(sample));
}

// Calculates confidence in a prediction by dividing the area of the predicted
// circle and dividing by the area of the circle representing distance covered
// Lower bounded by 0
double ConfidenceArc::predictConfidence() {
	double minor_rad = this->prediction.radius + EPSILON;
	double major_rad = norm(this->prediction.point - this->curr) + EPSILON;
	double conf = 1 - pow(minor_rad / major_rad, 2);
	return conf < 0 ? 0 : conf;
}
