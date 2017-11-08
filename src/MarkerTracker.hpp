#pragma once

#include <string>
#include <tuple>
#include <cstdlib>
#include <list>

#include "opencv2/objdetect.hpp"
#include "opencv2/tracking.hpp"

#include "MarkerType.hpp"
#include "MarkerMapper.hpp"
#include "FrameDerivatives.hpp"
#include "TrackerState.hpp"
#include "MarkerSeparator.hpp"
#include "EyeTracker.hpp"

using namespace std;
using namespace cv;

namespace YerFace {

class MarkerCandidate {
public:
	RotatedRect marker;
	unsigned int markerListIndex;
	double distanceFromPointOfInterest;
	double angleFromPointOfInterest;
	double sqrtArea;
};

class MarkerMapper;

class MarkerTracker {
public:
	MarkerTracker(MarkerType myMarkerType, MarkerMapper *myMarkerMapper, FrameDerivatives *myFrameDerivatives, MarkerSeparator *myMarkerSeparator, EyeTracker *myEyeTracker = NULL, float myTrackingBoxPercentage = 1.5, float myMaxTrackerDriftPercentage = 0.75);
	~MarkerTracker();
	MarkerType getMarkerType(void);
	TrackerState processCurrentFrame(void);
	void renderPreviewHUD(bool verbose = true);
	TrackerState getTrackerState(void);
	tuple<Point2d, bool> getMarkerPoint(void);
	static vector<MarkerTracker *> *getMarkerTrackers(void);
	static MarkerTracker *getMarkerTrackerByType(MarkerType markerType);
	static bool sortMarkerCandidatesByDistanceFromPointOfInterest(const MarkerCandidate a, const MarkerCandidate b);
	static bool sortMarkerCandidatesByAngleFromPointOfInterest(const MarkerCandidate a, const MarkerCandidate b);
	static bool sortMarkerCandidatesByAngleFromPointOfInterestInverted(const MarkerCandidate a, const MarkerCandidate b);
private:
	void performTrackToSeparatedCorrelation(void);
	void performDetection(void);
	void performInitializationOfTracker(void);
	bool performTracking(void);
	bool trackerDriftingExcessively(void);
	bool claimMarkerCandidate(MarkerCandidate markerCandidate);
	bool claimFirstAvailableMarkerCandidate(list<MarkerCandidate> *markerCandidateList);
	void assignMarkerPoint(void);
	void generateMarkerCandidateList(list<MarkerCandidate> *markerCandidateList, Point2d pointOfInterest, Rect2d *boundingRect = NULL);
	
	static vector<MarkerTracker *> markerTrackers;

	MarkerType markerType;
	MarkerMapper *markerMapper;
	FrameDerivatives *frameDerivatives;
	MarkerSeparator *markerSeparator;
	EyeTracker *eyeTracker;
	float trackingBoxPercentage;
	float maxTrackerDriftPercentage;

	Ptr<Tracker> tracker;
	vector<MarkerSeparated> *markerList;
	TrackerState trackerState;
	MarkerCandidate markerDetected;
	bool markerDetectedSet;
	Rect2d trackingBox;
	bool trackingBoxSet;
	Point2d markerPoint;
	bool markerPointSet;
};

}; //namespace YerFace
