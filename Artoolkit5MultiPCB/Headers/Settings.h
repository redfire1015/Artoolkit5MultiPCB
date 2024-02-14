#ifndef SET_def
#define SET_def

//********************************************************//
//						Dependancies
//********************************************************//
#include "string"

//********************************************************//
//						CLASS DEFINITION
//********************************************************//

class settings
{
public:
	// Constructors
	settings();

	settings(const std::string& filePath, const std::wstring& com, bool virtualOnly, float voltage,
		int testPoint, int waveForm, double simTime, double step,
		double start, double end);

	// Copy constructor
	settings(const settings& other);

	// Assignment operator
	settings& operator=(const settings& other);

	// Destructor
	~settings();

	// Getters
	std::string getPCBFilePath() const;
	std::wstring getComPort() const;
	bool getVirtualOnlySimulation() const;
	float getInputVoltage() const;
	int getSelectedTestPoint() const;
	int getSelectedWaveForm() const;
	double getSimulationTime() const;
	double getTimeStep() const;
	double getStartTime() const;
	double getEndTime() const;

	// Setters
	void setPCBFilePath(const std::string& path);
	void setComPort(const std::wstring& com);
	void setVirtualOnlySimulation(bool virtualOnly);
	void setInputVoltage(float voltage);
	void setSelectedTestPoint(int testPoint);
	void setSelectedWaveForm(int waveForm);
	void setSimulationTime(double simTime);
	void setTimeStep(double step);
	void setStartTime(double start);
	void setEndTime(double end);
private:
	std::string PCBFilePath;
	std::wstring comPort;
	bool virtual_only_simulation;
	float inputVoltage;
	int selectedTestPoint;
	int selectedWaveForm;
	double simulationTime;
	double timeStep;
	double startTime;
	double endTime;
};



#endif //!SET_def
