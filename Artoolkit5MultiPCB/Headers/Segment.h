#ifndef Segment_def
#define Segment_def

#include "XYCoord.h"
#include <string>

class Segment
{
public:
	// Default constructor
	Segment();

	// Parameterized constructor
	Segment(const XYCoord& startCoord_input, const XYCoord& endCoord_input, float thickness_input, const std::string& layer_input);

	// Copy constructor
	Segment(const Segment& other);

	// Copy assignment operator
	Segment& operator=(const Segment& other);

	// Getters for thickness and layer name
	float getSegmentThickness() const;

	// Accessor methods for start and end coordinates
	const XYCoord& getStartCoord() const;

	const XYCoord& getEndCoord() const;

	// Destructor
	~Segment();

private:
	XYCoord startCoord;     // Start XY
	XYCoord endCoord;       // End XY
	float SegmentThickness; // Thickness of a trace (aka Segment)
};

#endif