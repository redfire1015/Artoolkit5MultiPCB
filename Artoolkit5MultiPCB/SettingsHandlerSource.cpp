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
		else if (key == "Total Simulation Time") {
			readSettings.setSimulationTime(std::stof(value));
		}
		else if (key == "Simulation Time Step") {
			readSettings.setTimeStep(std::stod(value));
		}
		else if (key == "Simulation Start Time") {
			readSettings.setStartTime(std::stod(value));
		}
		else if (key == "Simulation End Time") {
			readSettings.setEndTime(std::stod(value));
		}
		//TODO:
		//else if (key == "Simulation End Time") {
		//	readSettings.setEndTime(std::stod(value));
		//}
		//else if (key == "Simulation End Time") {
		//	readSettings.setEndTime(std::stod(value));
		//}
		//else if (key == "Simulation End Time") {
		//	readSettings.setEndTime(std::stod(value));
		//}

	}
}


void refreshSettings() {

	//Extract pcbFilepath from settings file
	loadKicadPCBFile(markerConfigFilePath, readSettings.getPCBFilePath().c_str());

	if ((config = arMultiReadConfigFile(configName, arPattHandle)) == NULL) {
		ARLOGe("config data load error !!\n");
		exit(0);
	}
}