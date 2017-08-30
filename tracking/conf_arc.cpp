#include "conf_arc.hpp"

#define EPSILON 0.000001
#define DEF_BACKTRACE 5
#define DEF_RANGE 0.05
#define FRAME_DIAG 1468.8

using namespace std;
using namespace cv;

Prediction::Prediction() : Prediction(Point2f(0.0f, 0.0f), 0, 0, DEF_RANGE) { }

Prediction::Prediction(Point2f pt, double conf, double rad, double rng) {
	this->point = pt;
	this->confidence = conf;
	this->radius = rad;
	this->range = rng;
}

void Prediction::setRange(double rng) {
	this->range = rng;
}

ConfidenceArc::ConfidenceArc() : ConfidenceArc(Point2f(0.0f, 0.0f), Point2f(0.0f, 0.0f), DEF_BACKTRACE) { }

ConfidenceArc::ConfidenceArc(Point2f prev, Point2f curr) : ConfidenceArc(prev, curr, DEF_BACKTRACE) { }

ConfidenceArc::ConfidenceArc(Point2f prev, Point2f curr, int num) {
	this->prev = prev;
	this->curr = curr;
	this->backtrace = num;
	this->recordError(curr, prev);
	this->recordError(curr, prev);
	this->path.push_back(prev);
	this->path.push_back(curr);
	this->prediction = Prediction();
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

void ConfidenceArc::predictNextFrame(Point2f* curr) {
	this->path.push_back(*curr);
	this->prev = this->curr;
	this->curr = *curr;
	this->recordError(this->prediction.point, this->curr);
	this->predict();
}

void ConfidenceArc::recordError(Point2f p1, Point2f p2) {
	double dist = norm(p1 - p2), threshold = this->prediction.radius * this->prediction.range;
	this->errors.push_back(dist < threshold ? 0 : dist);
}

void ConfidenceArc::predict() {
	this->predictPoint();
	this->predictRadius();
	this->predictConfidence();
}

// Calculates point of a prediction via linear approximation
void ConfidenceArc::predictPoint() {
	this->prediction.point = 2 * this->curr - this->prev;
}

// Calculates radius of a prediction by sampling the errors from previous predictions
void ConfidenceArc::predictRadius() {
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
	this->prediction.radius = fabs(distribution(sample));
}

// Calculates confidence in a prediction by calculating the area of the predicted
// circle and dividing by the area of the circle representing distance covered
// Lower bounded by 0
void ConfidenceArc::predictConfidence() {
	double conf = 1 - 2 * this->prediction.radius / FRAME_DIAG;
	this->prediction.confidence = conf < 0 ? 0 : conf;
}
