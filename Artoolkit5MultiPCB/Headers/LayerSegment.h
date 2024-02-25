#ifndef Segment_def
#define Segment_def

#include "XYCoord.h"
#include <string>

class Segment
{
public:
	// Default constructor
	Segment();

	// Parametrized constructor
	Segment(const XYCoord& startCoord_input, const XYCoord& endCoord_input, float thickness_input, int segmentNet_input, const std::string& layer_input);

	// Copy constructor
	Segment(const Segment& other);

	// Copy assignment operator
	Segment& operator=(const Segment& other);

	// Getters for thickness and layer name
	float getSegmentThickness() const;

	// Getters for segment net
	int getSegmentNet() const;

	// Accessor methods for start and end coordinates
	const XYCoord& getStartCoord() const;

	const XYCoord& getEndCoord() const;

	void setSegmantThickness(float thickness_in);

	// Getters for segment net
	void setSegmentNet(int net_in);

	void setStartCoord(XYCoord Start_in);

	void SetEndCoord(XYCoord End_in);

	// Getter and setter for layerName
	const std::string& getLayer() const;

	void setLayer(const std::string& name);


	// Destructor
	~Segment();

private:
	XYCoord startCoord;     // Start XY
	XYCoord endCoord;       // End XY
	float SegmentThickness; // Thickness of a trace (aka Segment)
	std::string layer;	//layer the segment is on
	int segmentNet;		//net Assigned to the segment
};

#endif