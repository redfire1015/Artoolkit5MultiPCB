#include "Headers/Layer.h"


// Default constructor
Layer::Layer() : layerName(""), numberOfLayerSegments(0), numberOfLayerPolygons(0), layerSegments(nullptr), layerPolygons(nullptr)
{
}

// Constructor with parameters
Layer::Layer(const std::string& name, int segmentCount, int polygonCount)
	: layerName(name), numberOfLayerSegments(segmentCount), numberOfLayerPolygons(polygonCount),
	layerSegments(new layerSegment[segmentCount]), layerPolygons(new layerPolygon[polygonCount])
{
}

// Copy constructor
Layer::Layer(const Layer& other)
	: layerName(other.layerName), numberOfLayerSegments(other.numberOfLayerSegments), numberOfLayerPolygons(other.numberOfLayerPolygons)
{
	// Copy segments
	layerSegments = new layerSegment[numberOfLayerSegments];
	for (int i = 0; i < numberOfLayerSegments; ++i)
	{
		layerSegments[i] = other.layerSegments[i];
	}

	// Copy polygons
	layerPolygons = new layerPolygon[numberOfLayerPolygons];
	for (int i = 0; i < numberOfLayerPolygons; ++i)
	{
		layerPolygons[i] = other.layerPolygons[i];
	}
}

// Copy assignment operator
Layer& Layer::operator=(const Layer& other)
{
	if (this != &other)
	{
		// Deallocate existing arrays
		delete[] layerSegments;
		delete[] layerPolygons;

		// Copy data from 'other'
		layerName = other.layerName;
		numberOfLayerSegments = other.numberOfLayerSegments;
		numberOfLayerPolygons = other.numberOfLayerPolygons;

		// Allocate and copy segments
		layerSegments = new layerSegment[numberOfLayerSegments];
		for (int i = 0; i < numberOfLayerSegments; ++i)
		{
			layerSegments[i] = other.layerSegments[i];
		}

		// Allocate and copy polygons
		layerPolygons = new layerPolygon[numberOfLayerPolygons];
		for (int i = 0; i < numberOfLayerPolygons; ++i)
		{
			layerPolygons[i] = other.layerPolygons[i];
		}
	}
	return *this;
}

// Destructor
Layer::~Layer()
{
	// Deallocate dynamic arrays
	delete[] layerSegments;
	delete[] layerPolygons;
}

// Getter and setter for numberOfLayerSegments
int Layer::getNumberOfLayerSegments() const
{
	return numberOfLayerSegments;
}

void Layer::setNumberOfLayerSegments(int count)
{
	// Allocate a new array with the specified size
	layerSegment* newSegments = new layerSegment[count];

	// Copy existing segments to the new array (up to the minimum of old and new count)
	for (int i = 0; i < std::min(count, numberOfLayerSegments); ++i)
	{
		newSegments[i] = layerSegments[i];
	}

	// Deallocate the old array
	delete[] layerSegments;

	// Update the pointer and count
	layerSegments = newSegments;
	numberOfLayerSegments = count;
}

// Getter and setter for numberOfLayerPolygons
int Layer::getNumberOfLayerPolygons() const
{
	return numberOfLayerPolygons;
}

void Layer::setNumberOfLayerPolygons(int count)
{
	// Allocate a new array with the specified size
	layerPolygon* newPolygons = new layerPolygon[count];

	// Copy existing polygons to the new array (up to the minimum of old and new count)
	for (int i = 0; i < std::min(count, numberOfLayerPolygons); ++i)
	{
		newPolygons[i] = layerPolygons[i];
	}

	// Deallocate the old array
	delete[] layerPolygons;

	// Update the pointer and count
	layerPolygons = newPolygons;
	numberOfLayerPolygons = count;
}

// Getter and setter for layerName
const std::string& Layer::getLayerName() const
{
	return layerName;
}

void Layer::setLayerName(const std::string& name)
{
	layerName = name;
}

// Getter and setter for layerSegments
const layerSegment* Layer::getLayerSegments() const
{
	return layerSegments;
}

void Layer::setLayerSegments(const layerSegment* segments, int count)
{
	// Deallocate existing array
	delete[] layerSegments;

	// Copy data from the provided array
	layerSegments = new layerSegment[count];
	for (int i = 0; i < count; ++i)
	{
		layerSegments[i] = segments[i];
	}

	// Update the count
	numberOfLayerSegments = count;
}

// Getter and setter for layerPolygons
const layerPolygon* Layer::getLayerPolygons() const
{
	return layerPolygons;
}

void Layer::setLayerPolygons(const layerPolygon* polygons, int count)
{
	// Deallocate existing array
	delete[] layerPolygons;
	// Copy data from the provided array
	layerPolygons = new layerPolygon[count];
	for (int i = 0; i < count; ++i)
	{
		layerPolygons[i] = polygons[i];
	}

	// Update the count
	numberOfLayerPolygons = count;
}
