#include "Headers/PCBhandler.h"

void LoadPCB(PCB& input, const char* pcbFilePath) { //Needs to Be adjusted for locaztion of markers

	//Populating Segments 
	float segWidth = 0.25; //All segments have the same width for testing

	//This is the coordinates from KiCad so going to modify based on relative location

	XYCoord seg1Start(125.867349, 80.01);
	XYCoord seg1End(152.48, 80.01);

	XYCoord seg2Start(157.48, 80.01);
	XYCoord seg2End(166.37, 80.01);

	XYCoord seg3Start(103.007349, 80.01);
	XYCoord seg3End(110.627349, 80.01);

	XYCoord seg4Start(82.55, 80.01);
	XYCoord seg4End(102.87, 80.01);

	Segment segment1(seg1Start, seg1End, segWidth, "F.Cu");
	Segment segment2(seg2Start, seg2End, segWidth, "F.Cu");
	Segment segment3(seg3Start, seg3End, segWidth, "F.Cu");
	Segment segment4(seg4Start, seg4End, segWidth, "F.Cu");

	// Create an array of segments
	Segment segmentsArray[] = { segment1, segment2, segment3, segment4 };

	//Populating Polygons
	  /*Leave for now*/

	//Populating Layer	
	Layer layer1("F.Cu", 4, 0);
	Layer layer2("B.cu", 0, 0);

	Layer layer3("F.Adhesive", 0, 0);
	Layer layer4("B.Adhesive", 0, 0);

	Layer layer5("F.Paste", 0, 0);
	Layer layer6("B.Paste", 0, 0);

	Layer layer7("F.Silkscreen", 0, 0);
	Layer layer8("B.Silkscreen", 0, 0);

	Layer layer9("F.Mask", 0, 0);
	Layer layer10("B.Mask", 0, 0);

	layer1.setLayerSegments(segmentsArray, (sizeof(segmentsArray) / sizeof(Segment)));

	Layer layerArray[] = { layer1, layer2, layer3, layer4,layer5,layer6, layer7, layer8, layer9, layer10 };

	//Populating PCB
	int numberOfLayers = (sizeof(layerArray) / sizeof(Layer));

	// Dynamically allocate memory for the layerNames array
	std::string* layerNames = new std::string[numberOfLayers];

	for (int i = 0; i < numberOfLayers; i++)
	{
		layerNames[i] = layerArray[i].getLayerName();
	}

	input.setNumberOfLayers((sizeof(layerArray) / sizeof(Layer)));
	input.setPCBName("inputPCB");
	input.setPCBLayerNames(layerNames, input.getNumberOfLayers());
	input.setPCBLayers(layerArray, input.getNumberOfLayers());


	// // Test some getters
	//std::cout << "PCB Name: " << myPCB.getPCBName() << std::endl;

	//for (size_t i = 0; i < myPCB.getNumberOfLayers(); ++i)
	//{
	//	std::cout << "Layer Name: " << myPCB.getPCBLayerNames()[i] << std::endl;
	//	std::cout << "Number of Segments: " << myPCB.getPCBLayers()[i].getNumberOfLayerSegments() << std::endl;
	//	std::cout << "Number of Polygons: " << myPCB.getPCBLayers()[i].getNumberOfLayerPolygons() << std::endl;
	//}

	// Print information about segments and polygons
	//for (size_t i = 0; i < myPCB.getNumberOfLayers(); ++i)
	//{
	//	std::cout << "Layer Name: " << myPCB.getPCBLayerNames()[i] << std::endl;

	//	const Layer& currentLayer = myPCB.getPCBLayers()[i];

	//	// Print information about segments
	//	std::cout << "Segments:" << std::endl;
	//	for (int j = 0; j < currentLayer.getNumberOfLayerSegments(); ++j)
	//	{
	//		const Segment& currentSegment = currentLayer.getLayerSegments()[j];
	//		std::cout << "  Segment " << j + 1 << ": Thickness - " << currentSegment.getSegmentThickness() << ", Start Coord - ("
	//			<< currentSegment.getStartCoord().getXCoord() << ", " << currentSegment.getStartCoord().getYCoord() << "), End Coord - ("
	//			<< currentSegment.getEndCoord().getXCoord() << ", " << currentSegment.getEndCoord().getYCoord() << ")" << std::endl;
	//	}
	//	std::cout << std::endl; // Add a newline for better readability
	//}

	//Clearing Memory
	delete[] layerNames;
}






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
	for (const std::string& layerName : uniqueLayers) {
		// Count the number of segments for the current layer
		int numSegments = std::count_if(SegmentVector.begin(), SegmentVector.end(),
			[&layerName](const Segment& seg) { return seg.getLayer() == layerName; });

		// Extract segments for the current layer
		std::vector<Segment> layerSegments;
		std::copy_if(SegmentVector.begin(), SegmentVector.end(), std::back_inserter(layerSegments),
			[&layerName](const Segment& seg) { return seg.getLayer() == layerName; });

		// Create a Layer object and add it to the vector
		layers.emplace_back(layerName, numSegments, 0);
	}

	// Iterate through the layers and associate segments
	for (Layer& layer : layers) {
		// Extract segments for the current layer
		std::vector<Segment> layerSegments;
		std::copy_if(SegmentVector.begin(), SegmentVector.end(), std::back_inserter(layerSegments),
			[&layer](const Segment& seg) { return seg.getLayer() == layer.getLayerName(); });

		// Set the layer segments
		layer.setLayerSegments(layerSegments.data(), layerSegments.size());
	}

	//Load Segments into PCB Class
	loadedPCB.setNumberOfLayers(layers.size());
	loadedPCB.setPCBLayers(layers.data(), loadedPCB.getNumberOfLayers());

	//Process Markers 
	processMarker(fullFileString, markerFilePath);


	//Process Polygons
	//processPolygons(fullFile);


	//Debugging
	//std::cout << "Total Number of Segments:		" << std::endl;
	//std::cout << numSegs << std::endl;
	//std::cout << "Layer List:		" << std::endl;
	//for (const auto& str : layerList) {
	//	std::cout << str << " ";
	//}
	//std::cout << std::endl;
	//End Debugging

	pcbInputFile.close();
	fullFileString.clear();
}

Segment processSegment(std::string inputString) {

	std::cout << "Found Segment: " << inputString << std::endl;

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

