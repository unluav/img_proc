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
	this->distanceErrors.push_back(0.0);
	this->distanceErrors.push_back(0.0);
	this->angleErrors.push_back(M_PI);
	this->angleErrors.push_back(M_PI);
	this->predictNext();
}

vector<Point2f>* ConfidenceArc::getPath() {
	return &this->path;
}

Prediction* ConfidenceArc::getPrediction() {
	return &this->prediction;
}

vector<double>* ConfidenceArc::getDistanceErrors() {
	return &this->distanceErrors;
}

vector<double>* ConfidenceArc::getAngleErrors() {
	return &this->angleErrors;
}

// Returns the standard deviation and mean of a collection of doubles
// Length dictates how far back along the path the calculation should consider
// A length less than or eqaul to zero will use the whole path
pair<double, double> ConfidenceArc::calculateStats(vector<double>* collection, int length) {
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
	Point2f previous = this->path[this->path.size() - 2];
	Point2f current = this->path[this->path.size() - 1] - previous;
	Point2f predicted = this->prediction.point - previous;
	Point2f error = predicted - current;

	this->distanceErrors.push_back(norm(predicted) - norm(current));
	this->angleErrors.push_back(atan2(error.y, error.x));
}

// Straight-line predicts the next point
void ConfidenceArc::predictNext() {
	Point2f previous = this->path[this->path.size() - 2];
	Point2f current = this->path[this->path.size() - 1];
	
	this->prediction = Prediction(2 * current - previous, this->calculateConfidence());
}

// Calculates confidence in a prediction based on the last n points, given by length
double ConfidenceArc::calculateConfidence() {
	int length = 5;

	Point2f previous = this->path[this->path.size() - 2];
	Point2f current = this->path[this->path.size() - 1];
	pair<double, double> distanceStats = this->calculateStats(&this->distanceErrors, length);
	pair<double, double> angleStats = this->calculateStats(&this->angleErrors, length);

	double distance = norm(current - previous) + EPSILON;
	double distanceError = this->sampleError(&distanceStats) + EPSILON;
	double angleError = this->sampleError(&angleStats) + EPSILON;

	double arcArea = 4 * angleError * distance * distanceError;
	double circleArea = M_PI * pow(distance + distanceError, 2);

	return 1 - arcArea / circleArea;
}
