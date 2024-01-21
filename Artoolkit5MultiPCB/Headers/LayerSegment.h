#ifndef Segment_def
#define Segment_def

#include "XYCoord.h"
#include <string>

class layerSegment
{
public:
	// Default constructor
	layerSegment();

	// Parameterized constructor
	layerSegment(const XYCoord& startCoord_input, const XYCoord& endCoord_input, float thickness_input, const std::string& layer_input);

	// Copy constructor
	layerSegment(const layerSegment& other);

	// Copy assignment operator
	layerSegment& operator=(const layerSegment& other);

	// Getters for thickness and layer name
	float getSegmentThickness() const;

	// Accessor methods for start and end coordinates
	const XYCoord& getStartCoord() const;

	const XYCoord& getEndCoord() const;

	void setSegmantThickness(float thickness_in);

	void setStartCoord(XYCoord Start_in);

	void SetEndCoord(XYCoord End_in);

	// Destructor
	~layerSegment();

private:
	XYCoord startCoord;     // Start XY
	XYCoord endCoord;       // End XY
	float SegmentThickness; // Thickness of a trace (aka layerSegment)
};

#endif