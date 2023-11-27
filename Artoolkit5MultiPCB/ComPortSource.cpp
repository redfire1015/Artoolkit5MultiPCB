#include "Headers/ComPortHandler.h"

// Function to get the list of available COM ports
bool getComPorts(std::vector<std::string>& comPortList) {

	//Clear the current list
	comPortList.clear();

	char  lpTargetPath[5000]; // buffer to store the path of the COMPORTS
	bool gotPort = false; // in case the port is not found

	for (int i = 0; i < 255; i++) // checking ports from COM0 to COM255
	{
		std::string str = "COM" + std::to_string(i); // converting to COM0, COM1, COM2

		DWORD test = QueryDosDeviceA(str.c_str(), lpTargetPath, 5000);

		// Test the return value and error if any
		if (test != 0) //QueryDosDevice returns zero if it didn't find an object
		{
			// std::cout << str << ": " << lpTargetPath << std::endl; //Debugging
			comPortList.push_back(str); //Add to ComPortList
			gotPort = true;
		}

		if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
		}
	}

	return gotPort;
}