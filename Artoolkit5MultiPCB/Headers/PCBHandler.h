#ifndef PCBHandler_def
#define PCBHandler_def

#include "PCB.h"
#include <fstream>
#include <iomanip>  // Include for std::setprecision
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <sstream>

#include <vector>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <regex>

extern PCB loadedPCB;

/**
@param PCB File Path
@param Marker File Path
*/
void loadKicadPCBFile(char* markerFilePath, const char* pcbFilePath);

Segment processSegment(std::string inputString);

void processMarker(std::string fullFileString, char* markerFilePath);

void findAndCreateLayers(const std::string& inputString);


#endif
