#include "Headers/Settings.h"

// Constructors
settings::settings() : virtual_only_simulation(false), inputVoltage(0.0),
selectedTestPoint(0), selectedWaveForm(0),
simulationTime(0.0), timeStep(0.0), startTime(0.0), endTime(0.0)
{
	// Default constructor
}

settings::settings(const std::string& filePath, const std::wstring& com, bool virtualOnly, float voltage,
	int testPoint, int waveForm, double simTime, double step,
	double start, double end)
	: PCBFilePath(filePath), comPort(com), virtual_only_simulation(virtualOnly),
	inputVoltage(voltage), selectedTestPoint(testPoint),
	selectedWaveForm(waveForm), simulationTime(simTime),
	timeStep(step), startTime(start), endTime(end)
{
	// Parametrised constructor
}

// Copy constructor
settings::settings(const settings& other)
	: PCBFilePath(other.PCBFilePath), virtual_only_simulation(other.virtual_only_simulation),
	inputVoltage(other.inputVoltage), selectedTestPoint(other.selectedTestPoint),
	selectedWaveForm(other.selectedWaveForm), simulationTime(other.simulationTime),
	timeStep(other.timeStep), startTime(other.startTime), endTime(other.endTime)
{
	// Copy constructor
}

// Assignment operator
settings& settings::operator=(const settings& other)
{
	if (this != &other)
	{
		PCBFilePath = other.PCBFilePath;
		virtual_only_simulation = other.virtual_only_simulation;
		inputVoltage = other.inputVoltage;
		selectedTestPoint = other.selectedTestPoint;
		selectedWaveForm = other.selectedWaveForm;
		simulationTime = other.simulationTime;
		timeStep = other.timeStep;
		startTime = other.startTime;
		endTime = other.endTime;
	}
	return *this;
}

// Destructor
settings::~settings()
{
	// Destructor
}

// Getters
std::string settings::getPCBFilePath() const { return PCBFilePath; }
std::wstring settings::getComPort() const { return comPort; };
bool settings::getVirtualOnlySimulation() const { return virtual_only_simulation; }
float settings::getInputVoltage() const { return inputVoltage; }
int settings::getSelectedTestPoint() const { return selectedTestPoint; }
int settings::getSelectedWaveForm() const { return selectedWaveForm; }
double settings::getSimulationTime() const { return simulationTime; }
double settings::getTimeStep() const { return timeStep; }
double settings::getStartTime() const { return startTime; }
double settings::getEndTime() const { return endTime; }

// Setters
void settings::setPCBFilePath(const std::string& path) { PCBFilePath = path; }
void  settings::setComPort(const std::wstring& com) { comPort = com; };
void settings::setVirtualOnlySimulation(bool virtualOnly) { virtual_only_simulation = virtualOnly; }
void settings::setInputVoltage(float voltage) { inputVoltage = voltage; }
void settings::setSelectedTestPoint(int testPoint) { selectedTestPoint = testPoint; }
void settings::setSelectedWaveForm(int waveForm) { selectedWaveForm = waveForm; }
void settings::setSimulationTime(double simTime) { simulationTime = simTime; }
void settings::setTimeStep(double step) { timeStep = step; }
void settings::setStartTime(double start) { startTime = start; }
void settings::setEndTime(double end) { endTime = end; }