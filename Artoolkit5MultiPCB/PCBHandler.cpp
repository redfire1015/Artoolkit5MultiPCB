#include "Headers/PCBhandler.h"

PCB	loadedPCB;				//Externed inside PCBHandler

void loadKicadPCBFile(char* markerFilePath, const char* pcbFilePath)
{
	//Getting the Name of the PCB to name the PCB class
	// Find the last occurrence of '\\'
	const char* lastSlash = std::strrchr(pcbFilePath, '\\');

	if (lastSlash != nullptr) {
		// Extract the substring from lastSlash+1 to the end
		const char* extractedText = lastSlash + 1;
		loadedPCB.setPCBName(extractedText);

		std::cout << "Extracted text: " << extractedText << std::endl;
	}
	else {
		std::cerr << "No '\\' found in the file path." << std::endl;
	}

	//
	std::string fullFileString;
	std::ifstream pcbInputFile(pcbFilePath);

	if (!pcbInputFile.is_open())
	{
		std::cerr << "Error opening file" << std::endl;
		return;
	}

	std::string line;
	int numSegs = 0;

	std::vector<Segment>	SegmentVector;

	while (std::getline(pcbInputFile, line)) //Iterate through the whole file
	{
		//Note that KICAD separates segments onto individual lines
		//When we find a line with segment in
		if (line.find("  (segment ") != std::string::npos)
		{
			SegmentVector.push_back(processSegment(line)); //Process this line
			numSegs++;
		}
		fullFileString += line;
	}

	//Find List of all unique Layers
	// 
	//Collect unique layer names using an unordered_set
	//std::unordered_set<std::string> uniqueStrings;
	//for (const auto& layerName : SegmentVector) {
	//	uniqueStrings.insert(layerName.getLayer());
	//}

	////Display the total number of unique strings
	//std::cout << "Total number of unique strings: " << uniqueStrings.size() << "\n";

	// Use a set to store unique layer names
	std::set<std::string> uniqueLayers;

	// Iterate through the segments and add layer names to the set
	for (const Segment& currentSegment : SegmentVector) {
		uniqueLayers.insert(currentSegment.getLayer());
	}

	// Create a vector of Layer objects
	std::vector<Layer> layers;
	for (const std::string& currentLayer : uniqueLayers) {
		// Count the number of segments for the current layer
		int numSegments = std::count_if(SegmentVector.begin(), SegmentVector.end(),
			[&currentLayer](const Segment& seg) { return seg.getLayer() == currentLayer; });

		// Create a Layer object and add it to the vector
		layers.emplace_back(currentLayer, numSegments, 0);
	}

	// Iterate through the layers and associate segments
	for (Layer& currentLayer : layers) {
		// Extract segments for the current layer
		std::vector<Segment> layerSegments;
		std::copy_if(SegmentVector.begin(), SegmentVector.end(), std::back_inserter(layerSegments),
			[&currentLayer](const Segment& seg) { return seg.getLayer() == currentLayer.getLayerName(); });

		// Set the layer segments
		currentLayer.setLayerSegments(layerSegments.data(), layerSegments.size());
	}

	//Load Segments into PCB Class
	loadedPCB.setNumberOfLayers(layers.size());
	loadedPCB.setPCBLayers(layers.data(), loadedPCB.getNumberOfLayers());

	//Process Markers 
	processMarker(fullFileString, markerFilePath);


	//Process Polygons
	//processPolygons(fullFile);

	pcbInputFile.close();
	fullFileString.clear();
}

Segment processSegment(std::string inputString) {

	//std::cout << "Found Segment: " << inputString << std::endl;

	// Find and extract start x and y
	size_t startIndex = inputString.find("start");
	size_t endIndex = inputString.find(")", startIndex);
	std::string startSubstring = inputString.substr(startIndex, endIndex - startIndex);
	std::istringstream startStream(startSubstring);
	std::string startToken;
	double startX, startY;
	startStream >> startToken >> startX >> startY;

	// Find and extract end x and y
	size_t endIndexIndex = inputString.find("end");
	endIndex = inputString.find(")", endIndexIndex);
	std::string endSubstring = inputString.substr(endIndexIndex, endIndex - endIndexIndex);
	std::istringstream endStream(endSubstring);
	std::string endToken;
	double endX, endY;
	endStream >> endToken >> endX >> endY;

	// Find and extract width
	size_t widthIndex = inputString.find("width");
	endIndex = inputString.find(")", widthIndex);
	std::string widthSubstring = inputString.substr(widthIndex, endIndex - widthIndex);
	std::istringstream widthStream(widthSubstring);
	std::string widthToken;
	double width;
	widthStream >> widthToken >> width;

	// Find and extract layer name
	size_t layerIndex = inputString.find("layer");
	endIndex = inputString.find(")", layerIndex);
	std::string layerSubstring = inputString.substr(layerIndex, endIndex - layerIndex);
	size_t quote1 = layerSubstring.find("\"");
	size_t quote2 = layerSubstring.find("\"", quote1 + 1);
	std::string layerName = layerSubstring.substr(quote1 + 1, quote2 - quote1 - 1);

	// Find and extract net number
	size_t netIndex = inputString.find("net");
	endIndex = inputString.find(")", netIndex);
	std::string netSubstring = inputString.substr(netIndex, endIndex - netIndex);
	std::istringstream netStream(netSubstring);
	std::string netToken;
	int netNumber;
	netStream >> netToken >> netNumber;

	//Adding the Layer for this segment to a list 
	//findAndCreateLayers(layerName);

	// Print the extracted information
	//std::cout << "		SEGMENT DATA		" << std::endl;
	//std::cout << "Start X: " << startX << std::endl;
	//std::cout << "Start Y: " << startY << std::endl;
	//std::cout << "End X: " << endX << std::endl;
	//std::cout << "End Y: " << endY << std::endl;
	//std::cout << "Width: " << width << std::endl;
	//std::cout << "Layer: " << layerName << std::endl;
	//std::cout << "Net Number: " << netNumber << std::endl;
	//std::cout << "		END SEGMENT		" << std::endl;

	// 	Segment(const XYCoord& startCoord_input, const XYCoord& endCoord_input, float thickness_input, const std::string& layer_input);
	XYCoord startCoord(startX, startY);
	XYCoord endCoord(endX, endY);

	Segment tempSegment(startCoord, endCoord, width, layerName);

	return tempSegment;

}


void findAndCreateLayers(const std::string& inputString) {

}



void processMarker(std::string fullFileString, char* markerFilePath) {

	size_t markerIndex = 0;
	int numMarkers = 0;

	std::vector<int>			MarkerIDVector;
	XYCoord						MarkerCenterXY(0, 0);
	std::vector<XYCoord>		MarkerXYVector;

	// Process each marker in the input string
	while (true) {
		// Find and extract marker ID
		size_t makerIdIndex = fullFileString.find("Markers:marker", markerIndex); //Find location of marker in the text
		if (makerIdIndex == std::string::npos) { //If we can't find any more then finish
			break; // No more markers found
		}

		size_t makerIdEndIndex = fullFileString.find("\"", makerIdIndex + 14); //Find " in text
		std::string makerIdSubstring = fullFileString.substr(makerIdIndex + 14, makerIdEndIndex - makerIdIndex - 14); // Extract the marker ID substring
		std::istringstream makerIdStream(makerIdSubstring);// Create a stringstream for parsing the marker ID
		int makerId;// Variable to store the parsed marker ID
		makerIdStream >> makerId; // Extract the marker ID from the stringstream

		// Find and extract location
		size_t atIndex = fullFileString.find("(at", makerIdEndIndex);
		size_t locationEndIndex = fullFileString.find(")", atIndex);
		std::string locationSubstring = fullFileString.substr(atIndex, locationEndIndex - atIndex);
		std::istringstream locationStream(locationSubstring);
		std::string atToken;
		double locationX, locationY;
		locationStream >> atToken >> locationX >> locationY;
		MarkerCenterXY.setXYCoord(locationX, locationY);

		MarkerIDVector.push_back(makerId);
		MarkerXYVector.push_back(MarkerCenterXY);

		//// Display results for each marker
		//std::cout << "Marker ID: " << makerId << std::endl;
		//std::cout << "Location: (" << locationX * 2 << ", " << locationY * -2 << ")" << std::endl;

		// Move markerIndex to the next position
		markerIndex = locationEndIndex;
		numMarkers++;
	}

	// Create and write to the marker.dat file
	std::ofstream outputFile(markerFilePath);

	std::string rotationMatrixX = "1.0000 0.0000 0.0000 ";
	std::string rotationMatrixY = "0.0000 1.0000 0.0000 ";
	std::string rotationMatrixZ = "0.0000 0.0000 1.0000 0.0000"; //Z coord is always 0

	int scalingFactor = 2;
	int markerSize = 40; //Fixed Size (All markers are the same size so this value doesn't matter too much)

	if (outputFile.is_open()) {
		outputFile << numMarkers << std::endl;

		for (int i = 0; i < MarkerIDVector.size(); ++i) {

			outputFile << "#Marker " << MarkerIDVector[i] << std::endl;
			outputFile << std::setw(2) << std::setfill('0') << MarkerIDVector[i] << std::endl;
			outputFile << std::fixed << std::setprecision(1);
			outputFile << markerSize << std::endl;
			outputFile << std::fixed << std::setprecision(4);
			outputFile << rotationMatrixX << MarkerXYVector[i].getXCoord() * scalingFactor << std::endl; //Kicad has scaled the in the PCB editor by 2! Not sure how/ why this has happened
			outputFile << rotationMatrixY << MarkerXYVector[i].getYCoord() * -scalingFactor << std::endl;
			outputFile << rotationMatrixZ << std::endl;
			outputFile << "" << std::endl;
		}

		// Close the file
		outputFile.close();
		std::cout << "Data has been written to marker.dat." << std::endl;
	}
	else {
		std::cerr << "Unable to open the file." << std::endl;
	}

}

