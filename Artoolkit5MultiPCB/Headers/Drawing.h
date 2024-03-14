#ifndef DRAW_def
#define DRAW_def

//********************************************************//
//						Dependancies
//********************************************************//
#define GLEW_STATIC 
#include <GL/glew.h>

#include <GL/glut.h>
#include <AR/ar.h>
#include <AR/gsub.h>

#include <vector> 

#include "SimulationHandler.h"
#include "PCB.h"
#include "colourmaps.h"

extern PCB loadedPCB;
extern ARHandle* arHandle;

extern GLuint textureID;
extern GLuint VAO; // Vertex Array Object ID

//Vertices Struct
struct Vertex
{
	float pos[2];
	float col[4] = { 0.0,0.0,0.0,0.0 };
	float tex[1] = { 0.5 };
};

// Create an empty vector 
extern std::vector<Vertex> vertices;

//Setup Complete Checking Variable
extern bool setupComplete;

//********************************************************//
//						Function Definition
//********************************************************//

void loadVertexToGPU(); //TODO: Function that loads the PCB segments once

void setColoursAndTextures(const Layer& currentLayer, const Segment& currentSegment);

void drawPCB(ARdouble trans1[3][4]);


#endif //!DRAW_def
