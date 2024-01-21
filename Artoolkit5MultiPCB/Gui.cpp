#include "Headers/GUI.h"


bool espConnected = false;
bool settingConfigLoaded = false;


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
			MessageBox(NULL, szFileName, L"File Selected", MB_OK);
		}
		settingConfigLoaded = true;
		break;
	}

	case 2: { //Begin virtual only simulation
		if (settingConfigLoaded == false) {
			MessageBox(NULL, L"Load Settings Config before continuing!", L"Warning", MB_OK | MB_ICONWARNING); //Alert trying to run simulation before settings have been configured
		}
		break;
	}


	case 3: {
		// Begin virtual and esp simulation
		if (settingConfigLoaded == false) {
			MessageBox(NULL, L"Load Settings Config before continuing!", L"Warning", MB_OK | MB_ICONWARNING); //Alert trying to run simulation before settings have been configured
		}
		break;
	}
	}
}