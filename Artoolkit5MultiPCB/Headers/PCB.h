#ifndef PCB_def
#define PCB_def

#include <string>
#include "Layer.h"

class PCB
{
public:
	// Default constructor
	PCB();

	// Parameter constructor with arrays of layer names and layers
	PCB(const std::string& name, int layerCount, const std::string* layerNames, const Layer* layers);

	// Copy constructor with arrays of layer names and layers
	PCB(const PCB& other);

	// Copy assignment operator with arrays of layer names and layers
	PCB& operator=(const PCB& other);

	// Destructor
	~PCB();

	// Getters and setters for the number of layers
	int getNumberOfLayers() const;

	void setNumberOfLayers(int count);

	// Getters and setters for layer names
	const std::string* getPCBLayerNames() const;

	void setPCBLayerNames(const std::string* names, int count);

	const Layer* getPCBLayers() const;

	void setPCBLayers(const Layer* layers, int count);

	// Getters and setters for PCB name
	const std::string& getPCBName() const;

	void setPCBName(const std::string& name);

private:
	Layer* PCBLayers;           // Dynamic array of Layer objects
	std::string* PCBLayerNames; // Dynamic array of layer names
	int numberOfLayers;
	std::string PCBName;
};

#endif