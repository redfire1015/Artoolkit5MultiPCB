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
		double start, double end, int colMap, double minColMap, double maxColMap);

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
	int getColourMap() const;
	double getColourMapMin() const;
	double getColourMapMax() const;

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
	void setColourMap(int colMap);
	void setColourMapMin(double colMapMin);
	void setColourMapMax(double colMapMax);

private:
	std::string PCBFilePath;//
	std::wstring comPort;
	//TODO: Layers
	bool virtual_only_simulation;
	float inputVoltage;//
	int selectedTestPoint;//
	int selectedWaveForm;//
	double simulationTime;
	double timeStep;
	double startTime;
	double endTime;
	int colourMap;
	double minColourMap;
	double maxColourMap;
};



#endif //!SET_def
