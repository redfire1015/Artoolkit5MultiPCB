#ifndef PCBHandler_def
#define PCBHandler_def

#include "PCB.h"
#include <fstream>
#include <iomanip>  // Include for std::setprecision
#include <iostream>

void LoadPCB(PCB& input, const char* pcbFilePath);

double calculateDistance(const XYCoord& point1, const XYCoord& point2);

XYCoord LoadMarkerConfiguation(char* markerFilePath);


#endif
