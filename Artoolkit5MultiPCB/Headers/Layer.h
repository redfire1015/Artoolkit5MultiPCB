#ifndef Layer_def
#define layer_def

#include "LayerPolygons.h"
#include "LayerSegment.h"
#include <string>

class Layer
{
public:
	// Default constructor
	Layer();

	// Constructor with parameters
	Layer(const std::string& name, int segmentCount, int polygonCount);

	// Copy constructor
	Layer(const Layer& other);

	// Copy assignment operator
	Layer& operator=(const Layer& other);

	// Destructor
	~Layer();

	// Getter and setter for numberOfLayerSegments
	int getNumberOfLayerSegments() const;

	void setNumberOfLayerSegments(int count);

	// Getter and setter for numberOfLayerPolygons
	int getNumberOfLayerPolygons() const;

	void setNumberOfLayerPolygons(int count);

	// Getter and setter for layerName
	const std::string& getLayerName() const;

	void setLayerName(const std::string& name);

	// Getter and setter for layerSegments
	const Segment* getLayerSegments() const;

	void setLayerSegments(const Segment* segments, int count);

	// Getter and setter for layerPolygons
	const layerPolygon* getLayerPolygons() const;

	void setLayerPolygons(const layerPolygon* polygons, int count);

private:
	std::string layerName;
	int numberOfLayerSegments;
	int numberOfLayerPolygons;
	Segment* layerSegments;
	layerPolygon* layerPolygons;
};

#endif