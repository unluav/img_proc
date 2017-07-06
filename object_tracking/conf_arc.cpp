#include "conf_arc.hpp"

#define EPSILON 0.000001

using namespace std;
using namespace cv;

Prediction::Prediction() {
	this->point = Point2f(0.0f, 0.0f);
	this->confidence = 0;
}

Prediction::Prediction(Point2f p, double c) {
	this->point = p;
	this->confidence = c;
}

ConfidenceArc::ConfidenceArc() {
	Point2f origin(0.0f, 0.0f);
	this->construct(&origin, &origin);
}

ConfidenceArc::ConfidenceArc(Point2f* previous, Point2f* current) {
	this->construct(previous, current);
}

// Because calling the default constructor from another parameterized constructor
// is a bit touchy, I present to you my own, private constructor that's not
// actually a constructor in the technical sense
void ConfidenceArc::construct(Point2f* previous, Point2f* current) {
	this->path.push_back(*previous);
	this->path.push_back(*current);
	this->prev = *previous;
	this->curr = *current;
	this->dist_errors.push_back(0.0);
	this->dist_errors.push_back(0.0);
	this->angle_errors.push_back(M_PI);
	this->angle_errors.push_back(M_PI);
	this->predictNext();
}

vector<Point2f>* ConfidenceArc::getPath() {
	return &this->path;
}

Prediction* ConfidenceArc::getPrediction() {
	return &this->prediction;
}

vector<double>* ConfidenceArc::getDistanceErrors() {
	return &this->dist_errors;
}

vector<double>* ConfidenceArc::getAngleErrors() {
	return &this->angle_errors;
}

Point2f* ConfidenceArc::getPrevious() {
	return &this->prev;
}

Point2f* ConfidenceArc::getCurrent() {
	return &this->curr;
}

// Returns the standard deviation and mean of a collection of doubles
// Length dictates how far back along the path the calculation should consider
// A length less than or eqaul to zero will use the whole path
pair<double, double> ConfidenceArc::calcStats(vector<double>* collection, int length) {
	double mean = 0, variance = 0;
	int size = collection->size();
	length = length <= 0 ? size : min(length, size);

	for (int i = 1; i <= length; i++) {
		mean += (*collection)[size - i] / length;
	}

	for (int i = 1; i <= length; i++) {
		variance += pow((*collection)[size - i] - mean, 2);
	}

	return make_pair(sqrt(variance / (length <= 1 ? 1 : length - 1)), mean);
}

double ConfidenceArc::sampleError(pair<double, double>* stats) {
	default_random_engine sample;
	normal_distribution<double> distribution(stats->second, stats->first);
	return fabs(distribution(sample));
}

void ConfidenceArc::predictNextFrame(Point2f* current) {
	this->path.push_back(*current);
	this->prev = this->curr;
	this->curr = *current;
	this->recordError();
	this->predictNext();
}

void ConfidenceArc::predictNextFrame(vector<Point2f>* centers, vector<ConfidenceArc>* arcs) {
	for (int i = 0; i < centers->size(); i++) {
		(*arcs)[i].predictNextFrame(&(*centers)[i]);
	}
}

// Given the current recorded position of the roomba, calculates how incorrect
// the prediction was and pushes that to the histories of errors
void ConfidenceArc::recordError() {
	Point2f current = this->curr - this->prev;
	Point2f pred = this->prediction.point - this->prev;
	Point2f error = pred - current;

	this->dist_errors.push_back(norm(pred) - norm(current));
	this->angle_errors.push_back(atan2(error.y, error.x));
}

// Straight-line predicts the next point
void ConfidenceArc::predictNext() {
	this->prediction = Prediction(2 * this->curr - this->prev, this->calcConf());
}

// Calculates confidence in a prediction based on the last n points, given by length
double ConfidenceArc::calcConf() {
	int length = 5;

	pair<double, double> dist_stats = this->calcStats(&this->dist_errors, length);
	pair<double, double> angle_stats = this->calcStats(&this->angle_errors, length);

	double dist = norm(this->curr - this->prev) + EPSILON;
	double dist_error = this->sampleError(&dist_stats) + EPSILON;
	double angle_error = this->sampleError(&angle_stats) + EPSILON;

	double arc_area = 4 * angle_error * dist * dist_error;
	double circ_area = M_PI * pow(dist + dist_error, 2);

	return 1 - arc_area / circ_area;
}
