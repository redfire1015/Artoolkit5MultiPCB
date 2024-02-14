#ifndef SETHan_def
#define SETHan_def

//********************************************************//
//						Dependancies
//********************************************************//

#include "Settings.h"
#include "PCBHandler.h"
#include <AR/ar.h>
#include <AR/gsub.h>
#include <AR/arMulti.h>

#include "Windows.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <codecvt>

extern settings readSettings;

extern char* markerConfigFilePath; //Externed Here - Not exposed to User unlike PCB file path

extern ARMultiMarkerInfoT* config;
extern ARParam			cparam;
extern ARGViewport		viewport;
extern ARPattHandle* arPattHandle;
extern char				vconf[512];
extern char				configName[512];
extern AR_PIXEL_FORMAT	pixFormat;
extern int				i;
extern int				xsize, ysize;

//********************************************************//
//						FUNCTIONS
//********************************************************//

void loadSettings(const char* pcbFilePath);

void refreshSettings();


#endif //!SETHan_def
