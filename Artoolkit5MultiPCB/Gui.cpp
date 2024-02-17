#include "Headers/GUI.h"
#include "Headers/SimulationHandler.h"

bool espConnected = false;
bool settingConfigLoaded = false;

char* charFilePath = nullptr;

settings readSettings;

//********************************************************//
//					GLUT Menu Definition
//********************************************************//
void menu(int id)
{
	switch (id)
	{
	case 1: { //Load Settings Config
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
			//MessageBox(NULL, szFileName, L"File Selected", MB_OK);
			settingConfigLoaded = true;
		}


		//Converting Tchar to Char 
		// Calculate the size of the buffer needed
		int charCount = WideCharToMultiByte(CP_UTF8, 0, szFileName, -1, NULL, 0, NULL, NULL);

		// Allocate a buffer for the converted string
		charFilePath = new char[charCount];

		// Perform the conversion
		WideCharToMultiByte(CP_UTF8, 0, szFileName, -1, charFilePath, charCount, NULL, NULL);

		loadSettings(charFilePath);//Load Settings from the Selected File Path
		refreshSettings();

		break;
	}

	case 2: { //Refresh Already Loaded Settings File
		if (settingConfigLoaded == false) {
			MessageBox(NULL, L"Load Settings Config before continuing!", L"Warning", MB_OK | MB_ICONWARNING); //Alert trying to run simulation before settings have been configured
			break;
		}
		loadSettings(charFilePath);//Load Settings from the Selected File Path
		refreshSettings();
		break;
	}

	case 3: { //Begin virtual only simulation
		if (settingConfigLoaded == false) {
			MessageBox(NULL, L"Load Settings Config before continuing!", L"Warning", MB_OK | MB_ICONWARNING); //Alert trying to run simulation before settings have been configured
			break;
		}
		//Start Simulation
		// Get the current time point
		auto currentTime = std::chrono::system_clock::now();

		// Convert the current time point to milliseconds since epoch
		auto durationSinceEpoch = currentTime.time_since_epoch();

		// Convert the duration to milliseconds as a double
		simulationStartTime = std::chrono::duration<double, std::milli>(durationSinceEpoch).count();

		std::cout << "Current time in milliseconds since epoch: " << simulationStartTime << std::endl;



		break;
	}


	case 4: {
		// Begin virtual and esp simulation
		if (settingConfigLoaded == false) {
			MessageBox(NULL, L"Load Settings Config before continuing!", L"Warning", MB_OK | MB_ICONWARNING); //Alert trying to run simulation before settings have been configured
			break;
		}

		break;
	}
	}
}