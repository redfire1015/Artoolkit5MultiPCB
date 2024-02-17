#ifndef SimHand_def
#define SimHand_def
//********************************************************//
//						Dependancies
//********************************************************//
#include <iostream>
#include <chrono>

#include "SettingsHandler.h" //Gives Access to the loaded settings instance

//********************************************************//
//						Variables
//********************************************************//

extern std::chrono::system_clock::time_point currentTime;
extern std::chrono::system_clock::time_point previousTime;

extern double timeSinceSimulationStart;

extern bool simulationStarted;

//********************************************************//
//						FUNCTIONS
//********************************************************//


//Function to create the right click context menu
void runSimulation(double TimeSinceSimulationStart);

#endif