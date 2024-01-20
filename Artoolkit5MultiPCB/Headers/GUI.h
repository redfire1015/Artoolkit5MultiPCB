#ifndef GUI_def
#define GUI_def

//********************************************************//
//						Dependancies
//********************************************************//
#define NOMINMAX
#include "ComPortHandler.h"
#include "PCB.h"

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glut.h"
#include "IMGUI/imgui_impl_opengl2.h"
#include "IMGUI/imgui_stdlib.h"

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <Windows.h> //For System Handling

//********************************************************//
//			Defning Structs for PCB Display Control
//********************************************************//

struct PCBLayerControl {
	std::string name;
	bool display;
};

struct PCBDisplaySetting {
	std::vector<PCBLayerControl> LayerControl;
	// Add additional PCB settings here
};


//********************************************************//
//						FUNCTIONS
//********************************************************//

// Function to clamp a value between a minimum and maximum
float clamp(float value, float min, float max);

//Function to Create the settings Window
void createSettingsWindow();

void populateLayerOptions();

//Function to run the settings window
void runSettingsWindow();

//Function to create the right click context menu
void menu(int id);

#endif //!Guidef
