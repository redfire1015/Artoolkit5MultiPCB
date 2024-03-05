#ifndef DRAW_def
#define DRAW_def

//********************************************************//
//						Dependancies
//********************************************************//

#include <AR/ar.h>
#include <AR/gsub.h>
#include <GL/glut.h>


#include "SimulationHandler.h"
#include "PCB.h"
#include "colourmaps.h"

extern PCB loadedPCB;
extern ARHandle* arHandle;
extern GLuint textureID;

//********************************************************//
//						Function Definition
//********************************************************//

void loadVertexToGPU(); //TODO: Function that loads the PCB segments once

void drawPCB(ARdouble trans1[3][4]);


#endif //!DRAW_def
