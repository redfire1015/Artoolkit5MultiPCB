#include "Headers/Segment.h"

// Default constructor
Segment::Segment() : SegmentThickness(0.0), startCoord(), endCoord()
{
}

// Parameterized constructor
Segment::Segment(const XYCoord& startCoord_input, const XYCoord& endCoord_input, float thickness_input, const std::string& layer_input)
	: startCoord(startCoord_input), endCoord(endCoord_input), SegmentThickness(thickness_input) {}

// Copy constructor
Segment::Segment(const Segment& other)
	: startCoord(other.startCoord), endCoord(other.endCoord), SegmentThickness(other.SegmentThickness)
{
}

// Copy assignment operator
Segment& Segment::operator=(const Segment& other)
{
	if (this != &other)
	{
		startCoord = other.startCoord;
		endCoord = other.endCoord;
		SegmentThickness = other.SegmentThickness;
	}
	return *this;
}

// Getters for thickness and layer name
float Segment::getSegmentThickness() const { return SegmentThickness; }

// Accessor methods for start and end coordinates
const XYCoord& Segment::getStartCoord()  const { return startCoord; }

const XYCoord& Segment::getEndCoord()  const { return endCoord; }

void Segment::setSegmantThickness(float thickness_in)
{
	SegmentThickness = thickness_in;
}

void Segment::setStartCoord(XYCoord Start_in) {
	startCoord = Start_in;
}

void Segment::SetEndCoord(XYCoord End_in) {
	endCoord = End_in;
}

// Destructor
Segment::~Segment()
{
}
