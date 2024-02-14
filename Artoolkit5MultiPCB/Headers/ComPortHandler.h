#ifndef ComPort_def
#define ComPort_def

#define NOMINMAX

#include <vector>
#include <string>
#include <Windows.h>

// Function to get the list of available COM ports
bool getComPorts(std::vector<std::string>& comPortList); // -  Might not be needed

bool startESPSimulation();


#endif // !ComPort_def