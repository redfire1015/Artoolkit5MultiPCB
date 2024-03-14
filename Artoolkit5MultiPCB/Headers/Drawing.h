#ifndef DRAW_def
#define DRAW_def

//********************************************************//
//						Dependancies
//********************************************************//

#include <GL/glew.h>
#include <GL/glut.h>
#include <AR/ar.h>
#include <AR/gsub.h>

#include "SimulationHandler.h"
#include "PCB.h"
#include "colourmaps.h"

extern PCB loadedPCB;
extern ARHandle* arHandle;

extern GLuint textureID;
extern GLuint VAO; // Vertex Array Object ID

extern float* verticies;
//********************************************************//
//						Function Definition
//********************************************************//

void loadVertexToGPU(); //TODO: Function that loads the PCB segments once

void setColoursAndTextures(const Layer& currentLayer, const Segment& currentSegment);

void drawPCB(ARdouble trans1[3][4]);


#endif //!DRAW_def
