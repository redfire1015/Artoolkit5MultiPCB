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

// Function to calculate Euclidean distance between two points
double calculateDistance(const XYCoord& point1, const XYCoord& point2) {
	return std::sqrt(std::pow(point2.getXCoord() - point1.getXCoord(), 2) + std::pow(point2.getYCoord() - point1.getYCoord(), 2));
}

XYCoord LoadMarkerConfiguation(char* markerFilePath) {

	int scalingFactor = 2;

	//Marker Size 40 - Assume all the same size for now (can later be modified so size is calculated from PCB file)
	float markerSize = 40.0;
	//Load marker locations on PCB

	//Marker 0
	//X 77.47, Y 48.26
	XYCoord marker0Centre(77.47, 48.26);

	//Marker 1
	//X 168.91 Y 48.26
	XYCoord marker1Centre(168.91, 48.26);

	//Marker 2
	//X 77.47, Y 119.38
	XYCoord marker2Centre(77.47, 119.38);

	//Marker 3
	//X 168.91 Y 119.38
	XYCoord marker3Centre(168.91, 119.38);

	XYCoord markers[] = { marker0Centre, marker1Centre,marker2Centre,marker3Centre };
	int numberOfPCBMarker = sizeof(markers) / sizeof(markers[0]);

	//Calculate marker closest to top left - This will be the reference point
	XYCoord origin(0, 0);

	// Initialize variables for closest marker and distance
	int closestMarker = 0;
	double minDistance = calculateDistance(marker0Centre, origin); //Start with a basic value to compare to

	// Loop through all markers
	for (int i = 0; i < numberOfPCBMarker; ++i) {
		double distance = calculateDistance(marker0Centre, markers[i]);

		// Check if the current marker is closer
		if (distance < minDistance) {
			closestMarker = i;
			minDistance = distance;
		}
	}

	//Calculate relative location
	//KiCad sets the Top left location to be the origin.
	//Therefore the closest marker to the top left will be the origin for the marker coordinate system

	XYCoord originMarker = markers[closestMarker];

	printf("X: %f and Y:%f\n", originMarker.getXCoord(), originMarker.getYCoord());

	// Calculate relative locations
	XYCoord* relativeLocations = new XYCoord[numberOfPCBMarker];

	for (int i = 0; i < numberOfPCBMarker; i++) {
		float tempXCoord;
		float tempYCoord;

		//XCoord Processing
		//XCoord system the same as normal
		//tempXCoord = markers[i].getXCoord() - originMarker.getXCoord(); //X Relative Coords always Current X - Chosen Origin X
		tempXCoord = markers[i].getXCoord();

		//YCoord Processing 
		//YCoords flipped positve negative direction so have to process
		//tempYCoord = originMarker.getYCoord() - markers[i].getYCoord();
		tempYCoord = -markers[i].getYCoord();

		//relativeLocations[i].setXYCoord((markers[i].getXCoord() - originMarker.getXCoord()), (markers[i].getYCoord() - originMarker.getYCoord())); //WRONG!!
		relativeLocations[i].setXYCoord(tempXCoord, tempYCoord);
	}

	// Create and write to the marker.dat file
	std::ofstream outputFile(markerFilePath);

	std::string rotationMatrixX = "1.0000 0.0000 0.0000 ";
	std::string rotationMatrixY = "0.0000 1.0000 0.0000 ";
	std::string rotationMatrixZ = "0.0000 0.0000 1.0000 0.0000"; //Z coord is always 0

	if (outputFile.is_open()) {
		outputFile << numberOfPCBMarker << std::endl;

		for (int i = 0; i < numberOfPCBMarker; i++) {

			outputFile << "#Marker " << i << std::endl;
			outputFile << std::setw(2) << std::setfill('0') << i << std::endl;
			outputFile << std::fixed << std::setprecision(1);
			outputFile << markerSize << std::endl;
			outputFile << std::fixed << std::setprecision(4);
			outputFile << rotationMatrixX << relativeLocations[i].getXCoord() * scalingFactor << std::endl; //Kicad has scaled the in the PCB editor by 2! Not sure how/ why this has happened
			outputFile << rotationMatrixY << relativeLocations[i].getYCoord() * scalingFactor << std::endl;
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

	//return origin point for later use?
	return (originMarker);
}