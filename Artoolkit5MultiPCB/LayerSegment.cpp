#include "Headers/LayerSegment.h"

// Default constructor
Segment::Segment() : SegmentThickness(0.0), startCoord(), endCoord(), layer("")
{
}

// Parametrized constructor
Segment::Segment(const XYCoord& startCoord_input, const XYCoord& endCoord_input, float thickness_input, int segmentNet_input, const std::string& layer_input)
	: startCoord(startCoord_input), endCoord(endCoord_input), SegmentThickness(thickness_input), segmentNet(segmentNet_input), layer(layer_input) {}

// Copy constructor
Segment::Segment(const Segment& other)
	: startCoord(other.startCoord), endCoord(other.endCoord), SegmentThickness(other.SegmentThickness), segmentNet(other.segmentNet), layer(other.layer)
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
		segmentNet = other.segmentNet;
		layer = other.layer;
	}
	return *this;
}

// Getters for thickness and layer name
float Segment::getSegmentThickness() const { return SegmentThickness; }

// Getters for segment net
float Segment::getSegmentNet() const { return segmentNet; };

// Accessor methods for start and end coordinates
const XYCoord& Segment::getStartCoord()  const { return startCoord; }

const XYCoord& Segment::getEndCoord()  const { return endCoord; }

void Segment::setSegmantThickness(float thickness_in)
{
	SegmentThickness = thickness_in;
}

void Segment::setSegmentNet(int net_in)
{
	segmentNet = net_in;
}

void Segment::setStartCoord(XYCoord Start_in) {
	startCoord = Start_in;
}

void Segment::SetEndCoord(XYCoord End_in) {
	endCoord = End_in;
}

// Getter and setter for layerName
const std::string& Segment::getLayer() const
{
	return layer;
}

void Segment::setLayer(const std::string& name)
{
	layer = name;
}

// Destructor
Segment::~Segment()
{
}
