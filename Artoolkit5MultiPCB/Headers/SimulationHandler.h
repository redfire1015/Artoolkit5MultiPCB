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

extern int toggle1;
extern int toggle2;
extern int toggle3;
extern int toggle4;

//Matrix Variables
extern double C;
extern double R;
extern double Vs;

extern spMATRIX A;
extern double* A_aa;
extern ML_INT* A_ja;
extern ML_INT* A_ia;

extern spMATRIX M;
extern double* M_aa;
extern ML_INT* M_ja;
extern ML_INT* M_ia;

extern double* original_b;
extern int original_b_size;
extern double* b;
extern int b_size;
//End Matrix Variables


//Variables for transient Solving
extern double hd;			//default timestep
extern double prevTimestep;	//Previous TimeStep
extern spMATRIX LHS;

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

//Function to modify Input sources via Numbers
void modifySimulation(int index, int value);

//Function to create the right click context menu
void runSimulation(double timestep);

#endif