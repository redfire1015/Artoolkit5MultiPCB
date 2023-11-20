#ifndef Polygon_def
#define Polygon_def 

#include "XYCoord.h"
#include <algorithm>

class layerPolygon
{
public:
	layerPolygon();

	// Constructor with specified XYCoords
	layerPolygon(const XYCoord* points, size_t count, float thickness);

	// Copy constructor
	layerPolygon(const layerPolygon& other);

	// Copy assignment operator
	layerPolygon& operator=(const layerPolygon& other);

	// Add a new XYCoord to the polygon
	void addPoint(const XYCoord& point);

	// Setter for the number of points
	void setPointCount(size_t count);

	// Setter for polygon thickness
	void setPolygonThickness(float thickness);

	// Accessor for polygon thickness
	float getPolygonThickness() const;

	// Accessor for polygon points
	const XYCoord* getPolygonPoints() const;

	// Accessor for the number of points
	size_t getPointCount() const;

	~layerPolygon();

private:
	XYCoord* polygonPointCoords; // Dynamically allocated array of XYCoords
	float polygonThickness;
	size_t pointCount;
};

#endif