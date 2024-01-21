#include "Headers/LayerSegment.h"

// Default constructor
layerSegment::layerSegment() : SegmentThickness(0.0), startCoord(), endCoord()
{
}

// Parameterized constructor
layerSegment::layerSegment(const XYCoord& startCoord_input, const XYCoord& endCoord_input, float thickness_input, const std::string& layer_input)
	: startCoord(startCoord_input), endCoord(endCoord_input), SegmentThickness(thickness_input) {}

// Copy constructor
layerSegment::layerSegment(const layerSegment& other)
	: startCoord(other.startCoord), endCoord(other.endCoord), SegmentThickness(other.SegmentThickness)
{
}

// Copy assignment operator
layerSegment& layerSegment::operator=(const layerSegment& other)
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
float layerSegment::getSegmentThickness() const { return SegmentThickness; }

// Accessor methods for start and end coordinates
const XYCoord& layerSegment::getStartCoord()  const { return startCoord; }

const XYCoord& layerSegment::getEndCoord()  const { return endCoord; }

void layerSegment::setSegmantThickness(float thickness_in)
{
	SegmentThickness = thickness_in;
}

void layerSegment::setStartCoord(XYCoord Start_in) {
	startCoord = Start_in;
}

void layerSegment::SetEndCoord(XYCoord End_in) {
	endCoord = End_in;
}

// Destructor
layerSegment::~layerSegment()
{
}
