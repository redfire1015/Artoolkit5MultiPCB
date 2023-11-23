#define _CRT_SECURE_NO_WARNINGS

/*
 *  multiTest.c
 *
 *  gsub-based example code to demonstrate use of ARToolKit
 *  with multi-marker tracking.
 *
 *  Press '?' while running for help on available key commands.
 *
 *  Disclaimer: IMPORTANT:  This Daqri software is supplied to you by Daqri
 *  LLC ("Daqri") in consideration of your agreement to the following
 *  terms, and your use, installation, modification or redistribution of
 *  this Daqri software constitutes acceptance of these terms.  If you do
 *  not agree with these terms, please do not use, install, modify or
 *  redistribute this Daqri software.
 *
 *  In consideration of your agreement to abide by the following terms, and
 *  subject to these terms, Daqri grants you a personal, non-exclusive
 *  license, under Daqri's copyrights in this original Daqri software (the
 *  "Daqri Software"), to use, reproduce, modify and redistribute the Daqri
 *  Software, with or without modifications, in source and/or binary forms;
 *  provided that if you redistribute the Daqri Software in its entirety and
 *  without modifications, you must retain this notice and the following
 *  text and disclaimers in all such redistributions of the Daqri Software.
 *  Neither the name, trademarks, service marks or logos of Daqri LLC may
 *  be used to endorse or promote products derived from the Daqri Software
 *  without specific prior written permission from Daqri.  Except as
 *  expressly stated in this notice, no other rights or licenses, express or
 *  implied, are granted by Daqri herein, including but not limited to any
 *  patent rights that may be infringed by your derivative works or by other
 *  works in which the Daqri Software may be incorporated.
 *
 *  The Daqri Software is provided by Daqri on an "AS IS" basis.  DAQRI
 *  MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 *  THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE, REGARDING THE DAQRI SOFTWARE OR ITS USE AND
 *  OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 *
 *  IN NO EVENT SHALL DAQRI BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 *  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 *  MODIFICATION AND/OR DISTRIBUTION OF THE DAQRI SOFTWARE, HOWEVER CAUSED
 *  AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 *  STRICT LIABILITY OR OTHERWISE, EVEN IF DAQRI HAS BEEN ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  Copyright 2015 Daqri LLC. All Rights Reserved.
 *  Copyright 2002-2015 ARToolworks, Inc. All Rights Reserved.
 *
 *  Author(s): Hirokazu Kato, Philip Lamb.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __APPLE__
#  ifdef _WIN32
#    include <windows.h>
#  endif
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
#include <AR/ar.h>
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/arMulti.h>

 //#define                 CPARA_NAME       "Data/camera_para.dat"
 //#define                 CPARA_NAME       "Data/jamesCamCalib.dat"
#define                 CPARA_NAME       "Data/paulCamCalib.dat"

#define                 CONFIG_NAME      "Data/marker.dat"

 //My Includes
#include "Headers/PCBHandler.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glut.h"
#include "IMGUI/imgui_impl_opengl2.h"
//End My Includes

//My Global Variables
XYCoord originMarkerPos;
PCB loadedPCB;
int             xsize, ysize; //moved from init statement

//IMGUI Variables
// 
// Our state
static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

//End My Global Variables

ARHandle* arHandle;
AR3DHandle* ar3DHandle;
ARGViewportHandle* vp;
ARMultiMarkerInfoT* config;
int                     robustFlag = 0;
int                     count;
ARParamLT* gCparamLT = NULL;

static void             init(int argc, char* argv[]);
static void             cleanup(void);
static void             mainLoop(void);
static void             draw(ARdouble trans1[3][4], ARdouble trans2[3][4], int mode);
static void             keyEvent(unsigned char key, int x, int y);

//MY FUNCTIONS
void runSecondWindow()
{

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glutSwapBuffers();
	glutPostRedisplay();
}

void createSecondWindow() {
	glutInitWindowSize(1280, 720);
	glutCreateWindow("Setting Menu");

	// Setup GLUT display function
	// We will also call ImGui_ImplGLUT_InstallFuncs() to get all the other functions installed for us,
	// otherwise it is possible to install our own functions and call the imgui_impl_glut.h functions ourselves.
	glutDisplayFunc(runSecondWindow);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	// FIXME: Consider reworking this example to install our own GLUT funcs + forward calls ImGui_ImplGLUT_XXX ones, instead of using ImGui_ImplGLUT_InstallFuncs().
	ImGui_ImplGLUT_Init();
	ImGui_ImplOpenGL2_Init();

	// Install GLUT handlers (glutReshapeFunc(), glutMotionFunc(), glutPassiveMotionFunc(), glutMouseFunc(), glutKeyboardFunc() etc.)
	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	ImGui_ImplGLUT_InstallFuncs();


	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);

	// Main loop
	glutMainLoop();

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
}

void menu(int id)
{
	switch (id)
	{
	case 1: {
		// Code for Option 1
		OPENFILENAME ofn;
		TCHAR szFileName[MAX_PATH] = { 0 };

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = sizeof(szFileName);
		ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn) == TRUE)
		{
			// The user selected a file, and the file path is in szFileName
			// You can use szFileName for further processing
			MessageBox(NULL, szFileName, L"File Selected", MB_OK);
		}

		break;
	}

	case 2: {
		createSecondWindow();
		break;
	}
	}
}
//END MY FUNCTIONS

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	init(argc, argv);

	glutCreateMenu(menu);
	glutAddMenuEntry("Select a PCB file to Load", 1);	//Option 1
	glutAddMenuEntry("Select PCB Layers to Display", 2);//Option 2
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	argSetDispFunc(mainLoop, 1);
	argSetKeyFunc(keyEvent);
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

	for (i = 0; i < config->marker_num; i++) { //Only want to do this once for one marker
		if (config->marker[i].visible >= 0) draw(config->trans, config->marker[i].trans, 0); //Dont really need second transform matrix
		else                                 draw(config->trans, config->marker[i].trans, 1);
	}

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

		LoadPCB(loadedPCB, originMarkerPos); //Populates loadedPCB with PCB data

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

static void draw(ARdouble trans1[3][4], ARdouble trans2[3][4], int mode)
{
	ARdouble  gl_para[16];
	GLfloat   light_position[] = { 100.0f, -200.0f, 200.0f, 0.0f };
	GLfloat   light_ambi[] = { 0.1f, 0.1f, 0.1f, 0.0f };
	GLfloat   light_color[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat   mat_flash[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat   mat_flash_shiny[] = { 50.0f };
	GLfloat   mat_diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat   mat_diffuse1[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	int       debugMode;


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	/* load the camera transformation matrix */
	glMatrixMode(GL_MODELVIEW);
	argConvGlpara(trans1, gl_para);
#ifdef ARDOUBLE_IS_FLOAT
	glLoadMatrixf(gl_para);
#else
	glLoadMatrixd(gl_para);
#endif
	argConvGlpara(trans2, gl_para);
#ifdef ARDOUBLE_IS_FLOAT
	glMultMatrixf(gl_para);
#else
	//glMultMatrixd(gl_para); //Try commenting this out This is translation to marker location. Only want to draw from coordinate origin.
#endif

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);
	if (mode == 0) {
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);
	}
	else {
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse1);
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 20.0f);
	arGetDebugMode(arHandle, &debugMode);



	if (debugMode == 0) glutSolidCube(40.0);
	else                glutWireCube(40.0);


	glPopMatrix();
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
}
