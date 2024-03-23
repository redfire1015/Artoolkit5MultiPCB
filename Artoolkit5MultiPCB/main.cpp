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

//My Includes
#include "GL/glew.h"
#include "Headers/Drawing.h"
#include "Headers/PCBHandler.h"
#include "Headers/GUI.h"
#include "Headers/SettingsHandler.h"
//End My Includes

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


//Ar Toolkit Global Variables
ARHandle* arHandle;
AR3DHandle* ar3DHandle;
ARGViewportHandle* vp;
int                     robustFlag = 0;
int                     count;
ARParamLT* gCparamLT = NULL;

char* markerConfigFilePath;
char* cameraConfigFilePath;
//End Artoolkit Global Variables

//Ar Toolkit Init Global Variables
ARMultiMarkerInfoT* config;
ARParam         cparam;
ARGViewport     viewport;
ARPattHandle* arPattHandle;
char            vconf[512];
char            configName[512];
AR_PIXEL_FORMAT pixFormat;
int             i;
int				xsize, ysize;
//End Artoolkit Init Global Variables

//ArToolkitFunction Defs
static void             init(int argc, char* argv[]);
static void             cleanup(void);
static void             mainLoop(void);
static void             keyEvent(unsigned char key, int x, int y);
//End ArToolkitFunction Defs



int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	init(argc, argv);

	//Assign Glut Menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Select Settings config to load", 1);	//Option 1
	glutAddMenuEntry("Refresh Settings from File", 2);//Option 2
	glutAddMenuEntry("Begin virtual simulation only", 3);//Option 2
	glutAddMenuEntry("End virtual simulation", 4);//Option 2
	glutAddMenuEntry("Begin virtual simulation and ESP32 simulation", 5);//Option 2
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//End Glut Menu

	argSetDispFunc(mainLoop, 1);
	argSetKeyFunc(keyEvent);

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

	//if (key == 'd') {
	//	arGetDebugMode(arHandle, &debug);
	//	debug = 1 - debug;
	//	arSetDebugMode(arHandle, debug);
	//}

	//if (key == '1') {
	//	arGetDebugMode(arHandle, &debug);
	//	if (debug) {
	//		arGetLabelingThresh(arHandle, &thresh);
	//		thresh -= 5;
	//		if (thresh < 0) thresh = 0;
	//		arSetLabelingThresh(arHandle, thresh);
	//		ARLOG("thresh = %d\n", thresh);
	//	}
	//}
	//if (key == '2') {
	//	arGetDebugMode(arHandle, &debug);
	//	if (debug) {
	//		arGetLabelingThresh(arHandle, &thresh);
	//		thresh += 5;
	//		if (thresh > 255) thresh = 255;
	//		arSetLabelingThresh(arHandle, thresh);
	//		ARLOG("thresh = %d\n", thresh);
	//	}
	//}

	//if (key == ' ') {
	//	robustFlag = 1 - robustFlag;
	//	if (robustFlag) ARLOG("Robust estimation mode.\n");
	//	else             ARLOG("Normal estimation mode.\n");
	//}

	if (key == '1') {
		if (simulationStarted)
		{
			toggle1 = readSettings.getInputVoltage() - toggle1;
			modifySimulation(1, toggle1);
		}
	}
	if (key == '2') {
		if (simulationStarted)
		{
			toggle2 = readSettings.getInputVoltage() - toggle2;
			modifySimulation(4, toggle2);
		}
	}
	if (key == '3') {
		if (simulationStarted)
		{
			toggle3 = readSettings.getInputVoltage() - toggle3;
			modifySimulation(10, toggle3);
		}
	}
	if (key == '4') {
		if (simulationStarted)
		{
			toggle4 = readSettings.getInputVoltage() - toggle4;
			modifySimulation(19, toggle4);
		}
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

	//Setting Absolute File Paths
	char* resourcesDir = arUtilGetResourcesDirectoryPath(AR_UTIL_RESOURCES_DIRECTORY_BEHAVIOR_BEST);
	// Allocate memory for the combined path
	markerConfigFilePath = (char*)malloc(strlen(resourcesDir) + strlen("\\") + strlen(CONFIG_NAME) + 1);
	cameraConfigFilePath = (char*)malloc(strlen(resourcesDir) + strlen("\\") + strlen(CPARA_NAME) + 1);

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

		//Get Settings File data
		//TODO: 
		//Exit program if file empty

		//loadSettings();

		//loadKicadPCBFile(markerConfigFilePath, "C:\\Users\\jrhol\\OneDrive\\Documents\\University_Of_Nottingham\\EEE\\Year_4\\EEEE4008_Individual_Project\\PCB's\\AR Basic Example\\BasicRCForGerberFile.kicad_pcb");
		//loadKicadPCBFile(markerConfigFilePath, "C:\\Users\\jrhol\\OneDrive\\Documents\\University_Of_Nottingham\\EEE\\Year_4\\EEEE4008_Individual_Project\\PCB's\\Interactive Example\\Proposal Example PCB\\Proposal Example PCB.kicad_pcb");

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

	//OpenGL Enables
	//Enable Colour Blending for glcolor4f
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable texturing
	glEnable(GL_TEXTURE_1D);
}
/* cleanup function called when program exits */
static void cleanup(void)
{
	arParamLTFree(&gCparamLT);
	arVideoCapStop();
	arVideoClose();
	argCleanup();
}
