#include "Headers/LayerPolygons.h"


layerPolygon::layerPolygon() : polygonThickness(0.0), pointCount(0), polygonPointCoords(nullptr)
{
}

// Constructor with specified XYCoords
layerPolygon::layerPolygon(const XYCoord* points, size_t count, float thickness)
	: polygonThickness(thickness), pointCount(count)
{
	polygonPointCoords = new XYCoord[count];
	for (size_t i = 0; i < count; ++i)
	{
		polygonPointCoords[i] = points[i];
	}
}
// Copy constructor
layerPolygon::layerPolygon(const layerPolygon& other)
	: polygonThickness(other.polygonThickness), pointCount(other.pointCount)
{
	polygonPointCoords = new XYCoord[pointCount];
	for (size_t i = 0; i < pointCount; ++i)
	{
		polygonPointCoords[i] = other.polygonPointCoords[i];
	}
}

// Copy assignment operator
layerPolygon& layerPolygon::operator=(const layerPolygon& other)
{
	if (this != &other)
	{
		// Deallocate existing array
		delete[] polygonPointCoords;

		// Copy data from 'other'
		polygonThickness = other.polygonThickness;
		pointCount = other.pointCount;

		// Allocate and copy array
		polygonPointCoords = new XYCoord[pointCount];
		for (size_t i = 0; i < pointCount; ++i)
		{
			polygonPointCoords[i] = other.polygonPointCoords[i];
		}
	}
	return *this;
}

// Add a new XYCoord to the polygon
void layerPolygon::addPoint(const XYCoord& point)
{
	// Allocate additional space for the new point
	XYCoord* newCoords = new XYCoord[pointCount + 1];

	// Copy existing points to the new array
	for (size_t i = 0; i < pointCount; ++i)
	{
		newCoords[i] = polygonPointCoords[i];
	}

	// Add the new point
	newCoords[pointCount] = point;

	// Deallocate the old array
	delete[] polygonPointCoords;

	// Update the pointer and count
	polygonPointCoords = newCoords;
	++pointCount;
}

// Setter for the number of points
void layerPolygon::setPointCount(size_t count)
{
	// Allocate a new array with the specified size
	XYCoord* newCoords = new XYCoord[count];

	// Copy existing points to the new array (up to the minimum of old and new count)
	for (size_t i = 0; i < std::min(count, pointCount); ++i)
	{
		newCoords[i] = polygonPointCoords[i];
	}

	// Deallocate the old array
	delete[] polygonPointCoords;

	// Update the pointer and count
	polygonPointCoords = newCoords;
	pointCount = count;
}

// Setter for polygon thickness
void layerPolygon::setPolygonThickness(float thickness)
{
	polygonThickness = thickness;
}

// Accessor for polygon thickness
float layerPolygon::getPolygonThickness() const
{
	return polygonThickness;
}

// Accessor for polygon points
const XYCoord* layerPolygon::getPolygonPoints() const
{
	return polygonPointCoords;
}

// Accessor for the number of points
size_t layerPolygon::getPointCount() const
{
	return pointCount;
}

layerPolygon::~layerPolygon()
{
	// Deallocate the dynamically allocated array
	delete[] polygonPointCoords;
}

