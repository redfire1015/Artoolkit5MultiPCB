#include "Headers/SettingsHandler.h"

settings readSettings;

void loadSettings(const char* settingsFilePath) {

	//Read settings into readSettings object
	std::ifstream file(settingsFilePath);
	std::string line;

	if (file.is_open()) {

	}
	else {
		std::cerr << "Unable to open file: " << settingsFilePath << std::endl;
	}

	while (getline(file, line)) {
		std::istringstream iss(line);
		std::string key, value;


		// Read the key until the first ':'
		getline(iss, key, ':');
		getline(iss, value);

		std::cout << "Key: " << key << ", Value: " << value << std::endl;

		if (key == "File Path") {
			readSettings.setPCBFilePath(value);
		}
		if (key == "Com Port") {

			// Convert narrow string to wide string
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::wstring wideStr = converter.from_bytes(value);
			readSettings.setComPort(wideStr); //TODO: note that this will be blank when in virtual only simulation, so when virtual only simulation is set to true, ignore this
		}
		else if (key == "Virtual Only Simulation?") {
			readSettings.setVirtualOnlySimulation(value == "True");
		}
		else if (key == "Input Voltage") {
			readSettings.setInputVoltage(std::stof(value));
		}
		else if (key == "Selected Test Point") {
			readSettings.setSelectedTestPoint(std::stoi(value));
		}
		else if (key == "Selected Wave Form") {
			readSettings.setSelectedWaveForm(std::stoi(value));
		}
		//else if (key == "Total Simulation Time") {
		//	readSettings.setSimulationTime(std::stof(value));
		//}
		//else if (key == "Simulation Time Step") {
		//	readSettings.setTimeStep(std::stod(value));
		//}
		//else if (key == "Simulation Start Time") {
		//	readSettings.setStartTime(std::stod(value));
		//}
		//else if (key == "Simulation End Time") {
		//	readSettings.setEndTime(std::stod(value));
		//}
		//TODO:
		else if (key == "Colour Map") {
			readSettings.setColourMap(stoi(value));
		}
		else if (key == "Colour Map Min") {
			readSettings.setColourMapMin(stoi(value));
		}
		else if (key == "Colour Map Max") {
			readSettings.setColourMapMax(stoi(value));
		}
	}
}

#define GL_CLAMP_TO_EDGE	0x812F //Added 

void refreshSettings() {

	//Extract pcbFilepath from settings file
	loadKicadPCBFile(markerConfigFilePath, readSettings.getPCBFilePath().c_str());

	//Update marker config
	if ((config = arMultiReadConfigFile(configName, arPattHandle)) == NULL) {
		ARLOGe("config data load error !!\n");
		exit(0);
	}

	// Generate and bind a texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_1D, textureID);

	// Load texture data
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 512, 0, GL_RGBA, GL_FLOAT, GUI_getColourMap(readSettings.getColourMap()));
	// Set texture parameters
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//Can Use linear or nearest although nearest makes sense as we are using the same texture for the entire vertex. 
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//Additionally linear can cause problems during the simulation start when the values are slightly negative
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_1D, 0);
}