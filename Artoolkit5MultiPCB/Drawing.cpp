#include "Headers/Drawing.h"

GLuint textureID;

void drawPCB(ARdouble trans1[3][4]) {

	ARdouble  gl_para[16];
	int       debugMode;

	/* load the camera transformation matrix */
	glMatrixMode(GL_MODELVIEW);
	argConvGlpara(trans1, gl_para);
#ifdef ARDOUBLE_IS_FLOAT
	glLoadMatrixf(gl_para);
#else
	glLoadMatrixd(gl_para);
#endif

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 10.0f);
	arGetDebugMode(arHandle, &debugMode);


	if (simulationStarted)
	{
		//Get the Current time
		currentTime = std::chrono::system_clock::now();

		//Calculate Time Step between last function call
		double timeStep = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count();
		previousTime = currentTime;

		// Calculate the difference
		timeSinceSimulationStart += timeStep;

		//std::cout << "Time since simulation start: " << timeSinceSimulationStart / 1000.0 << std::endl;

		//std::cout << transientNextSolution[30] << std::endl;


		runSimulation(timeStep);
	}

	//Print information about segments and polygons
	for (size_t i = 0; i < loadedPCB.getNumberOfLayers(); ++i)
	{

		const Layer& currentLayer = loadedPCB.getPCBLayers()[i];
		// Print information about segments
		for (int j = 0; j < currentLayer.getNumberOfLayerSegments(); ++j)
		{
			const Segment& currentSegment = currentLayer.getLayerSegments()[j];

			setColoursAndTextures(currentLayer, currentSegment);

			//glLineWidth(currentSegment.getSegmentThickness() * 7);  // Change this value based on your default line thickness
			//glBegin(GL_LINES);
			//glColor3f(1.0, 1.0, 1.0);  // White color, change values as needed
			//glVertex2f(currentSegment.getStartCoord().getXCoord() * 2, currentSegment.getStartCoord().getYCoord() * -2); //Start XY
			//glVertex2f(currentSegment.getEndCoord().getXCoord() * 2, currentSegment.getEndCoord().getYCoord() * -2); //End XY
			//glEnd();
			//glRectf(-currentSegment.getStartCoord().getXCoord() * 2, currentSegment.getEndCoord().getXCoord() * 2, currentSegment.getStartCoord().getYCoord() * -2, currentSegment.getEndCoord().getYCoord() * -2);

			// Calculate the direction vector
			float x1 = currentSegment.getStartCoord().getXCoord() * 2;
			float x2 = currentSegment.getEndCoord().getXCoord() * 2;
			float y1 = currentSegment.getStartCoord().getYCoord() * -2;
			float y2 = currentSegment.getEndCoord().getYCoord() * -2;

			float dx = x2 - x1;
			float dy = y2 - y1;

			// Calculate the length of the line
			float length = sqrt(dx * dx + dy * dy);

			// Normalize the direction vector
			dx /= length;
			dy /= length;

			// Calculate perpendicular vector
			float px = -dy;
			float py = dx;

			// Calculate half thickness
			float halfThickness = currentSegment.getSegmentThickness() / 2.0f;

			// Calculate the four vertices of the rectangle
			float x1t = x1 + px * halfThickness;
			float y1t = y1 + py * halfThickness;

			float x2t = x1 - px * halfThickness;
			float y2t = y1 - py * halfThickness;

			float x3t = x2 + px * halfThickness;
			float y3t = y2 + py * halfThickness;

			float x4t = x2 - px * halfThickness;
			float y4t = y2 - py * halfThickness;


			glBegin(GL_TRIANGLES);
			// Draw the rectangle using GL_TRIANGLES
			glVertex2f(x1t, y1t);
			glVertex2f(x2t, y2t);
			glVertex2f(x3t, y3t);

			glVertex2f(x2t, y2t);
			glVertex2f(x3t, y3t);
			glVertex2f(x4t, y4t);
			glEnd();
		}
	}

	glPopMatrix();

}

void setColoursAndTextures(const Layer& currentLayer, const Segment& currentSegment) {

	if (simulationStarted == false) {
		glBindTexture(GL_TEXTURE_1D, 0);	//Unbind any Active Tectures
		if (currentLayer.getLayerName() == "F.Cu") {
			glColor4f(1.0, 0.0, 0.0, 1.0); // Set colour to red
		}
		if (currentLayer.getLayerName() == "B.Cu") {
			glColor4f(0.0, 0.0, 1.0, 1.0); // Set colour to blue
		}
	}

	if (simulationStarted == true) {	 //TODO: Testing for simulation voltage

		float textureCoord;
		float scaled;

		switch (readSettings.getSelectedTestPoint()) {
		case 0:
			glBindTexture(GL_TEXTURE_1D, 0); //unbind any active textures
			if (currentSegment.getSegmentNet() == 22)
			{
				textureCoord = max(min(original_b[1], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else if (currentSegment.getSegmentNet() == 20)
			{
				textureCoord = max(min(transientCurrentSolution[2], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else
			{
				glBindTexture(GL_TEXTURE_1D, 0); //Unbind any active textures
				if (currentLayer.getLayerName() == "F.Cu") {
					glColor4f(1.0, 0.0, 0.0, 1.0); // Set colour to red
				}
				if (currentLayer.getLayerName() == "B.Cu") {
					glColor4f(0.0, 0.0,
						1.0, 1.0); // Set colour to blue
				}
			}
			break;
		case 1:
			glBindTexture(GL_TEXTURE_1D, 0); //unbind any active textures
			if (currentSegment.getSegmentNet() == 23)
			{
				textureCoord = max(min(original_b[4], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else if (currentSegment.getSegmentNet() == 21)
			{
				textureCoord = max(min(transientCurrentSolution[7], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else if (currentSegment.getSegmentNet() == 2)
			{
				textureCoord = max(min(transientCurrentSolution[8], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else
			{
				glBindTexture(GL_TEXTURE_1D, 0); //Unbind any active textures
				if (currentLayer.getLayerName() == "F.Cu") {
					glColor4f(1.0, 0.0, 0.0, 1.0); // Set colour to red
				}
				if (currentLayer.getLayerName() == "B.Cu") {
					glColor4f(0.0, 0.0, 1.0, 1.0); // Set colour to blue
				}
			}
			break;
		case 2:
			glBindTexture(GL_TEXTURE_1D, 0); //unbind any active textures
			if (currentSegment.getSegmentNet() == 24)
			{
				textureCoord = max(min(original_b[10], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else if (currentSegment.getSegmentNet() == 3)
			{
				textureCoord = max(min(transientCurrentSolution[15], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else if (currentSegment.getSegmentNet() == 4)
			{
				textureCoord = max(min(transientCurrentSolution[16], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else if (currentSegment.getSegmentNet() == 5)
			{
				textureCoord = max(min(transientCurrentSolution[17], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else
			{
				glBindTexture(GL_TEXTURE_1D, 0); //Unbind any active textures
				if (currentLayer.getLayerName() == "F.Cu") {
					glColor4f(1.0, 0.0, 0.0, 1.0); // Set colour to red
				}
				if (currentLayer.getLayerName() == "B.Cu") {
					glColor4f(0.0, 0.0, 1.0, 1.0); // Set colour to blue
				}
			}
			break;
		case 3:
			glBindTexture(GL_TEXTURE_1D, 0); //unbind any active textures
			if (currentSegment.getSegmentNet() == 19)
			{
				textureCoord = max(min(original_b[19], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else if (currentSegment.getSegmentNet() == 6)
			{
				//glColor4f(0.0, 1.0, 0.0, transientCurrentSolution[30] / 12)

				textureCoord = max(min(transientCurrentSolution[30], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else if (currentSegment.getSegmentNet() == 7)
			{
				//glColor4f(0.0, 1.0, 0.0, transientCurrentSolution[31] / 12);

				textureCoord = max(min(transientCurrentSolution[31], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else if (currentSegment.getSegmentNet() == 8)
			{
				//glColor4f(0.0, 1.0, 0.0, transientCurrentSolution[32] / 12);
				textureCoord = max(min(transientCurrentSolution[32], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else if (currentSegment.getSegmentNet() == 9)
			{
				//glColor4f(0.0, 1.0, 0.0, transientCurrentSolution[33] / 12);
				textureCoord = max(min(transientCurrentSolution[33], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else if (currentSegment.getSegmentNet() == 10)
			{
				//glColor4f(0.0, 1.0, 0.0, transientCurrentSolution[34] / 12);
				textureCoord = max(min(transientCurrentSolution[34], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else if (currentSegment.getSegmentNet() == 11)
			{
				//glColor4f(0.0, 1.0, 0.0, transientCurrentSolution[35] / 12);
				textureCoord = max(min(transientCurrentSolution[35], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				glBindTexture(GL_TEXTURE_1D, textureID);
				glTexCoord1f(scaled);
			}
			else
			{
				glBindTexture(GL_TEXTURE_1D, 0); //Unbind any active textures
				if (currentLayer.getLayerName() == "F.Cu") {
					glColor4f(1.0, 0.0, 0.0, 1.0); // Set colour to red
				}
				if (currentLayer.getLayerName() == "B.Cu") {
					glColor4f(0.0, 0.0, 1.0, 1.0); // Set colour to blue
				}
			}
			break;
		default:
			exit(-1); //Big Error if we get here
			break;
		}
	}
}