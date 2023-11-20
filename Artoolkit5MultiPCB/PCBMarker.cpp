#include "Headers/PCBMarker.h"

// Default constructor
PCBMarker::PCBMarker() : size(0.0), centreCoord()
{
}

// Parameterized constructor
PCBMarker::PCBMarker(const XYCoord& startCoord_input, const XYCoord& endCoord_input, float size_input)
	: centreCoord(startCoord_input), size(size_input) {}

// Copy constructor
PCBMarker::PCBMarker(const PCBMarker& other)
	: centreCoord(other.centreCoord), size(other.size)
{
}

// Copy assignment operator
PCBMarker& PCBMarker::operator=(const PCBMarker& other)
{
	if (this != &other)
	{
		centreCoord = other.centreCoord;
		size = other.size;
	}
	return *this;
}

// Accessor methods for start and end coordinates
const XYCoord& PCBMarker::getCentreCoord() const {
	return(centreCoord);
}

// Accessor methods for start and end coordinates
const XYCoord& PCBMarker::getMarkerSize() const {
	XYCoord placeholder;
	return(placeholder); //TODO:
}

// Destructor
PCBMarker::~PCBMarker()
{
}
