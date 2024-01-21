#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
/*
 *  multiTest.c
 *
 *  Press '?' while running for help on available key commands.
 *
 *  Copyright 2015 Daqri LLC. All Rights Reserved.
 *  Copyright 2002-2015 ARToolworks, Inc. All Rights Reserved.
 *
 *  Author(s): Hirokazu Kato, Philip Lamb.
 *
 */

 //Standard Library Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
//End Standard Library Includes

//Graphics and Platform Specific Includes
#ifndef __APPLE__
#  ifdef _WIN32
#    include <windows.h>
#  endif
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

//End Platform Specific includes

//Ar Toolkit includes
#include <AR/ar.h>
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/arMulti.h>
//End ArToolkit Includes

//ArToolkit Parameter Defines
 //#define                 CPARA_NAME       "Data/camera_para.dat"
 //#define                 CPARA_NAME       "Data/jamesCamCalib.dat"
#define                 CPARA_NAME       "Data/paulCamCalib.dat"
#define                 CONFIG_NAME      "Data/marker.dat"
//End ARToolkit Parameter Defines

//My Includes
#include "Headers/PCBHandler.h"
#include "Headers/GUI.h"
//End My Includes

//My Global Variables
XYCoord originMarkerPos;
PCB		loadedPCB;
int     xsize, ysize; //moved from init statement

//End My Global Variables

//Ar Toolkit Global Variables
ARHandle* arHandle;
AR3DHandle* ar3DHandle;
ARGViewportHandle* vp;
ARMultiMarkerInfoT* config;
int                     robustFlag = 0;
int                     count;
ARParamLT* gCparamLT = NULL;
//End Artoolkit Global Variables

//ArToolkitFunction Defs
static void             init(int argc, char* argv[]);
static void             cleanup(void);
static void             mainLoop(void);
static void             keyEvent(unsigned char key, int x, int y);
//End ArToolkitFunction Defs


//MY FUNCTIONS
void drawPCB(ARdouble trans1[3][4]);
//END MY FUNCTIONS


int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	init(argc, argv);

	argSetDispFunc(mainLoop, 1);
	argSetKeyFunc(keyEvent);

	//Assign Glut Menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Select Settings config to load", 1);	//Option 1
	glutAddMenuEntry("Begin virtual simulation only", 2);//Option 2
	glutAddMenuEntry("Begin virtual simulation and ESP32 simulation", 3);//Option 2
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//End Glut Menu

	//glutIdleFunc(mainLoop); // Needed when using free glut not sure why

	count = 0;
	arVideoCapStart();
	arUtilTimerReset();
	argMainLoop();
	return (0);
}

static void   keyEvent(unsigned char key, int x, int y)
{
	int     debug;
	int     thresh;

	/* quit if the ESC key is pressed */
	if (key == 0x1b) {
		ARLOG("*** %f (frame/sec)\n", (double)count / arUtilTimer());
		cleanup();
		exit(0);
	}

	if (key == 'd') {
		arGetDebugMode(arHandle, &debug);
		debug = 1 - debug;
		arSetDebugMode(arHandle, debug);
	}

	if (key == '1') {
		arGetDebugMode(arHandle, &debug);
		if (debug) {
			arGetLabelingThresh(arHandle, &thresh);
			thresh -= 5;
			if (thresh < 0) thresh = 0;
			arSetLabelingThresh(arHandle, thresh);
			ARLOG("thresh = %d\n", thresh);
		}
	}
	if (key == '2') {
		arGetDebugMode(arHandle, &debug);
		if (debug) {
			arGetLabelingThresh(arHandle, &thresh);
			thresh += 5;
			if (thresh > 255) thresh = 255;
			arSetLabelingThresh(arHandle, thresh);
			ARLOG("thresh = %d\n", thresh);
		}
	}

	if (key == ' ') {
		robustFlag = 1 - robustFlag;
		if (robustFlag) ARLOG("Robust estimation mode.\n");
		else             ARLOG("Normal estimation mode.\n");
	}
}

/* main loop */
static void mainLoop(void)
{
	ARUint8* dataPtr;
	ARMarkerInfo* marker_info;
	int             marker_num;
	int             imageProcMode;
	int             debugMode;
	double          err;
	int             i;

	/* grab a video frame */
	if ((dataPtr = (ARUint8*)arVideoGetImage()) == NULL) {
		arUtilSleep(2);
		return;
	}

	if (count == 100) {
		ARLOG("*** %f (frame/sec)\n", (double)count / arUtilTimer());
		arUtilTimerReset();
		count = 0;
	}
	count++;

	/* detect the markers in the video frame */
	if (arDetectMarker(arHandle, dataPtr) < 0) {
		cleanup();
		exit(0);
	}
	marker_num = arGetMarkerNum(arHandle);
	marker_info = arGetMarker(arHandle);

	argDrawMode2D(vp);
	arGetDebugMode(arHandle, &debugMode);
	if (debugMode == 0) {
		argDrawImage(dataPtr);
	}
	else {
		arGetImageProcMode(arHandle, &imageProcMode);
		if (imageProcMode == AR_IMAGE_PROC_FRAME_IMAGE) {
			argDrawImage(arHandle->labelInfo.bwImage);
		}
		else {
			argDrawImageHalf(arHandle->labelInfo.bwImage);
		}
		glColor3f(1.0f, 0.0f, 0.0f);
		glLineWidth(2.0f);
		for (i = 0; i < marker_num; i++) {
			argDrawSquareByIdealPos(marker_info[i].vertex);
		}
		glLineWidth(1.0f);
	}

	if (robustFlag) {
		err = arGetTransMatMultiSquareRobust(ar3DHandle, marker_info, marker_num, config);
	}
	else {
		err = arGetTransMatMultiSquare(ar3DHandle, marker_info, marker_num, config);
	}
	if (config->prevF == 0) {
		argSwapBuffers();
		return;
	}
	//ARLOGd("err = %f\n", err);

	argDrawMode3D(vp);
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);

	//for (i = 0; i < config->marker_num; i++) { //Only want to do this once for one marker
	//	if (config->marker[i].visible >= 0) draw(config->trans, config->marker[i].trans, 0); //Dont really need second transform matrix
	//	else                                 draw(config->trans, config->marker[i].trans, 1); //First transform is for overall, second is for indicidual.
	//}

	drawPCB(config->trans);
	argSwapBuffers();
}

static void   init(int argc, char* argv[])
{
	ARParam         cparam;
	ARGViewport     viewport;
	ARPattHandle* arPattHandle;
	char            vconf[512];
	char            configName[512];
	AR_PIXEL_FORMAT pixFormat;
	int             i;

	//Setting Absolute File Paths
	char* resourcesDir = arUtilGetResourcesDirectoryPath(AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_BEST);
	// Allocate memory for the combined path
	char* markerConfigFilePath = (char*)malloc(strlen(resourcesDir) + strlen("\\") + strlen(CONFIG_NAME) + 1);
	char* cameraConfigFilePath = (char*)malloc(strlen(resourcesDir) + strlen("\\") + strlen(CPARA_NAME) + 1);

	if (resourcesDir != NULL) {
		// Check if memory allocation was successful
		if (markerConfigFilePath != NULL && cameraConfigFilePath != NULL) {
			// Copy the resourcesDir to filePath
			strcpy(markerConfigFilePath, resourcesDir);

			// Copy the resourcesDir to filePath
			strcpy(cameraConfigFilePath, resourcesDir);

			// Concatenate "\\here.txt" to filePath
			strcat(markerConfigFilePath, "\\");
			strcat(markerConfigFilePath, CONFIG_NAME);


			// Concatenate "\\here.txt" to filePath
			strcat(cameraConfigFilePath, "\\");
			strcat(cameraConfigFilePath, CPARA_NAME);

			// Now filePath contains the full path including "\\here.txt"
			printf("Full path: %s\n", markerConfigFilePath);

			// Now filePath contains the full path including "\\here.txt"
			printf("Full path: %s\n", cameraConfigFilePath);
		}
		else {
			// Handle memory allocation failure
			printf("Memory allocation failed.\n");
		}

		//int scalingFactor = 2; //May be used
		originMarkerPos = LoadMarkerConfiguation(markerConfigFilePath);

		LoadPCB(loadedPCB, "Data"); //Populates loadedPCB with PCB data

		configName[0] = '\0';
		vconf[0] = '\0';
		for (i = 1; i < argc; i++) {
			if (strncmp(argv[i], "-config=", 8) == 0) {
				strcpy(configName, &argv[i][8]);
			}
			else {
				if (vconf[0] != '\0') strcat(vconf, " ");
				strcat(vconf, argv[i]);
			}
		}
		if (configName[0] == '\0') strcpy(configName, markerConfigFilePath);

		/* open the video path */
		if (arVideoOpen(vconf) < 0) exit(0);
		/* find the size of the window */
		if (arVideoGetSize(&xsize, &ysize) < 0) exit(0);
		ARLOGi("Image size (x,y) = (%d,%d)\n", xsize, ysize);
		if ((pixFormat = arVideoGetPixelFormat()) < 0) exit(0);

		/* set the initial camera parameters */
		if (arParamLoad(cameraConfigFilePath, 1, &cparam) < 0) {
			ARLOGe("Camera parameter load error !!\n");
			exit(0);
		}
		arParamChangeSize(&cparam, xsize, ysize, &cparam);
		ARLOG("*** Camera Parameter ***\n");
		arParamDisp(&cparam);
		if ((gCparamLT = arParamLTCreate(&cparam, AR_PARAM_LT_DEFAULT_OFFSET)) == NULL) {
			ARLOGe("Error: arParamLTCreate.\n");
			exit(-1);
		}

		if ((arHandle = arCreateHandle(gCparamLT)) == NULL) {
			ARLOGe("Error: arCreateHandle.\n");
			exit(0);
		}
		if (arSetPixelFormat(arHandle, pixFormat) < 0) {
			ARLOGe("Error: arSetPixelFormat.\n");
			exit(0);
		}

		if ((ar3DHandle = ar3DCreateHandle(&cparam)) == NULL) {
			ARLOGe("Error: ar3DCreateHandle.\n");
			exit(0);
		}

		if ((arPattHandle = arPattCreateHandle()) == NULL) {
			ARLOGe("Error: arPattCreateHandle.\n");
			exit(0);
		}
		arPattAttach(arHandle, arPattHandle);

		if ((config = arMultiReadConfigFile(configName, arPattHandle)) == NULL) {
			ARLOGe("config data load error !!\n");
			exit(0);
		}
		if (config->patt_type == AR_MULTI_PATTERN_DETECTION_MODE_TEMPLATE) {
			arSetPatternDetectionMode(arHandle, AR_TEMPLATE_MATCHING_COLOR);
		}
		else if (config->patt_type == AR_MULTI_PATTERN_DETECTION_MODE_MATRIX) {
			arSetPatternDetectionMode(arHandle, AR_MATRIX_CODE_DETECTION);
		}
		else { // AR_MULTI_PATTERN_DETECTION_MODE_TEMPLATE_AND_MATRIX
			arSetPatternDetectionMode(arHandle, AR_TEMPLATE_MATCHING_COLOR_AND_MATRIX);
		}

		/* open the graphics window */
		viewport.sx = 0;
		viewport.sy = 0;
		viewport.xsize = xsize;
		viewport.ysize = ysize;
		if ((vp = argCreateViewport(&viewport)) == NULL) exit(0);
		argViewportSetCparam(vp, &cparam);
		argViewportSetPixFormat(vp, pixFormat);
	}
}
/* cleanup function called when program exits */
static void cleanup(void)
{
	arParamLTFree(&gCparamLT);
	arVideoCapStop();
	arVideoClose();
	argCleanup();
}

void drawPCB(ARdouble trans1[3][4]) {

	ARdouble  gl_para[16];
	int       debugMode;

	/* load the camera transformation matrix */
	glMatrixMode(GL_MODELVIEW);
	argConvGlpara(trans1, gl_para);
#ifdef ARDOUBLE_IS_FLOAT
	glLoadMatrixf(gl_para);
#else
	glLoadMatrixd(gl_para);
#endif

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 10.0f);
	arGetDebugMode(arHandle, &debugMode);

	//std::cout << glGetError() << std::endl;
	// 
	//float lineWidth[2];
	//glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);
	//gets Supported widths and returns into  array - Not in this case we are told 0.5-10 on GTX1060 on windows 11

	//Print information about segments and polygons
	for (size_t i = 0; i < loadedPCB.getNumberOfLayers(); ++i)
	{
		const Layer& currentLayer = loadedPCB.getPCBLayers()[i];
		// Print information about segments
		for (int j = 0; j < currentLayer.getNumberOfLayerSegments(); ++j)
		{
			const Segment& currentSegment = currentLayer.getLayerSegments()[j];
			//currentSegment.getSegmentThickness()
			if (currentLayer.getLayerName() == "F.Cu") {
				//glLineWidth(currentSegment.getSegmentThickness() * 7);  // Change this value based on your default line thickness
				//glBegin(GL_LINES);
				//glColor3f(1.0, 1.0, 1.0);  // White color, change values as needed
				//glVertex2f(currentSegment.getStartCoord().getXCoord() * 2, currentSegment.getStartCoord().getYCoord() * -2); //Start XY
				//glVertex2f(currentSegment.getEndCoord().getXCoord() * 2, currentSegment.getEndCoord().getYCoord() * -2); //End XY
				//glEnd();
				//glRectf(-currentSegment.getStartCoord().getXCoord() * 2, currentSegment.getEndCoord().getXCoord() * 2, currentSegment.getStartCoord().getYCoord() * -2, currentSegment.getEndCoord().getYCoord() * -2);

				// Calculate the direction vector
				glColor3f(1.0, 0.0, 0.0); // Set color to red
				float x1 = currentSegment.getStartCoord().getXCoord() * 2;
				float x2 = currentSegment.getEndCoord().getXCoord() * 2;
				float y1 = currentSegment.getStartCoord().getYCoord() * -2;
				float y2 = currentSegment.getEndCoord().getYCoord() * -2;

				float dx = x2 - x1;
				float dy = y2 - y1;

				// Calculate the length of the line
				float length = sqrt(dx * dx + dy * dy);

				// Normalize the direction vector
				dx /= length;
				dy /= length;

				// Calculate perpendicular vector
				float px = -dy;
				float py = dx;

				// Calculate half thickness
				float halfThickness = currentSegment.getSegmentThickness() * 4 / 2.0f;

				// Calculate the four vertices of the rectangle
				float x1t = x1 + px * halfThickness;
				float y1t = y1 + py * halfThickness;

				float x2t = x1 - px * halfThickness;
				float y2t = y1 - py * halfThickness;

				float x3t = x2 + px * halfThickness;
				float y3t = y2 + py * halfThickness;

				float x4t = x2 - px * halfThickness;
				float y4t = y2 - py * halfThickness;

				// Draw the rectangle using GL_TRIANGLES
				glBegin(GL_TRIANGLES);
				glVertex2f(x1t, y1t);
				glVertex2f(x2t, y2t);
				glVertex2f(x3t, y3t);

				glVertex2f(x2t, y2t);
				glVertex2f(x3t, y3t);
				glVertex2f(x4t, y4t);
				glEnd();


			}

		}
	}

	glPopMatrix();

}