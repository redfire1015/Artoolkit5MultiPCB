#ifndef SimHand_def
#define SimHand_def

//********************************************************//
//						Dependancies
//********************************************************//
#include <iostream>
#include <chrono>

#include "SettingsHandler.h" //Gives Access to the loaded settings instance
#include "standAloneSolve.h"

//********************************************************//
//						Variables
//********************************************************//

//Matrix Variables
extern double C;
extern double R;
extern double Vs;

extern spMATRIX A;
extern spMATRIX M;
extern double b[];
//End Matrix Variables



//Variables for transient Solving
extern double hd;
extern double h;
extern spMATRIX LHS;

extern ML_INT mat_i, mat_j, mat_k1, mat_k2, mat_j1, mat_j2, mat_jl;
extern ML_INT* ia;
extern ML_INT* mind, * aind;

extern double* transientCurrentSolution;
extern double* transientNextSolution;
//End variables for transient Solving



//Timing Variables
extern std::chrono::system_clock::time_point currentTime;
extern std::chrono::system_clock::time_point previousTime;

extern double timeSinceSimulationStart;

extern bool simulationStarted;
//End Timing Variables


//********************************************************//
//						FUNCTIONS
//********************************************************//

void populateMatrix();

//Function to create the right click context menu
void runSimulation(double timestep);

#endif