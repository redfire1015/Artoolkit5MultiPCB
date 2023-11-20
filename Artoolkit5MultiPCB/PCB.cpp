#include "Headers/PCB.h"

//#define _CRT_SECURE_NO_WARNINGS
////System Headers
//#include <iostream>
//
////Class Headers
//#include "Headers/PCB.h"
//#include "Headers/PCBHandler.h"

// Default constructor
PCB::PCB() : PCBLayers(nullptr), PCBLayerNames(nullptr), numberOfLayers(0), PCBName("") {}

// Parameter constructor with arrays of layer names and layers
PCB::PCB(const std::string& name, int layerCount, const std::string* layerNames, const Layer* layers)
	: numberOfLayers(layerCount), PCBName(name)
{
	PCBLayers = new Layer[layerCount];
	PCBLayerNames = new std::string[layerCount];

	for (int i = 0; i < layerCount; ++i)
	{
		PCBLayers[i] = layers[i];
		PCBLayerNames[i] = layerNames[i];
	}
}

// Copy constructor with arrays of layer names and layers
PCB::PCB(const PCB& other)
	: numberOfLayers(other.numberOfLayers), PCBName(other.PCBName)
{
	PCBLayers = new Layer[numberOfLayers];
	PCBLayerNames = new std::string[numberOfLayers];

	for (int i = 0; i < numberOfLayers; ++i)
	{
		PCBLayers[i] = other.PCBLayers[i];
		PCBLayerNames[i] = other.PCBLayerNames[i];
	}
}

// Copy assignment operator with arrays of layer names and layers
PCB& PCB::operator=(const PCB& other)
{
	if (this != &other)
	{
		// Deallocate existing arrays
		delete[] PCBLayers;
		delete[] PCBLayerNames;

		// Copy data from 'other'
		numberOfLayers = other.numberOfLayers;
		PCBName = other.PCBName;

		// Allocate and copy layers and layer names
		PCBLayers = new Layer[numberOfLayers];
		PCBLayerNames = new std::string[numberOfLayers];

		for (int i = 0; i < numberOfLayers; ++i)
		{
			PCBLayers[i] = other.PCBLayers[i];
			PCBLayerNames[i] = other.PCBLayerNames[i];
		}
	}
	return *this;
}

// Destructor
PCB::~PCB()
{
	// Deallocate dynamic arrays
	delete[] PCBLayers;
	delete[] PCBLayerNames;
}

// Getters and setters for the number of layers
int PCB::getNumberOfLayers() const
{
	return numberOfLayers;
}

void PCB::setNumberOfLayers(int count)
{
	// Allocate new arrays with the specified size
	Layer* newLayers = new Layer[count];
	std::string* newLayerNames = new std::string[count];

	// Copy existing layers and layer names to the new arrays (up to the minimum of old and new count)
	for (int i = 0; i < std::min(count, numberOfLayers); ++i)
	{
		newLayers[i] = PCBLayers[i];
		newLayerNames[i] = PCBLayerNames[i];
	}

	// Deallocate the old arrays
	delete[] PCBLayers;
	delete[] PCBLayerNames;

	// Update the pointers and count
	PCBLayers = newLayers;
	PCBLayerNames = newLayerNames;
	numberOfLayers = count;
}

// Getters and setters for layer names
const std::string* PCB::getPCBLayerNames() const
{
	return PCBLayerNames;
}

void PCB::setPCBLayerNames(const std::string* names, int count)
{
	// Deallocate existing array
	delete[] PCBLayerNames;

	// Copy data from the provided array
	PCBLayerNames = new std::string[count];
	for (int i = 0; i < count; ++i)
	{
		PCBLayerNames[i] = names[i];
	}

	// Update the count
	numberOfLayers = count;
}

const Layer* PCB::getPCBLayers() const
{
	return PCBLayers;
}

void PCB::setPCBLayers(const Layer* layers, int count)
{
	// Deallocate existing array
	delete[] PCB::PCBLayers;

	// Copy data from the provided array
	PCBLayers = new Layer[count];
	for (int i = 0; i < count; ++i)
	{
		PCBLayers[i] = layers[i];
	}

	// Update the count
	numberOfLayers = count;
}

// Getters and setters for PCB name
const std::string& PCB::getPCBName() const
{
	return PCBName;
}

void PCB::setPCBName(const std::string& name)
{
	PCBName = name;
}


