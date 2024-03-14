#include "Headers/Drawing.h"

GLuint textureID;
GLuint VAO;		//Vertex Array Object ID

std::vector<Vertex> vertices;

bool setupComplete = false;

//TODO: Function that loads the PCB segments once
void loadVertexToGPU() {

	//Load Segments into Array
	for (size_t i = 0; i < loadedPCB.getNumberOfLayers(); ++i)
	{
		const Layer& currentLayer = loadedPCB.getPCBLayers()[i];

		for (int j = 0; j < currentLayer.getNumberOfLayerSegments(); ++j)
		{
			const Segment& currentSegment = currentLayer.getLayerSegments()[j];

			setColoursAndTextures(currentLayer, currentSegment);

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

			Vertex v1 = { {x1t, y1t} };
			Vertex v2 = { {x2t, y2t} };
			Vertex v3 = { {x3t, y3t} };
			Vertex v4 = { {x4t, y4t} };

			if (currentLayer.getLayerName() == "F.Cu") {

				float red[] = { 1.0 ,0.0,0.0,1.0 };
				for (int k = 0; k < sizeof(v1.col) / sizeof(v1.col[0]); k++)
				{
					v1.col[k] = red[k];
					v2.col[k] = red[k];
					v3.col[k] = red[k];
					v4.col[k] = red[k];
				}

			}
			if (currentLayer.getLayerName() == "B.Cu") {
				float blue[] = { 0.0 ,0.0,1.0,1.0 };
				for (int k = 0; k < sizeof(v1.col) / sizeof(v1.col[0]); k++)
				{
					v1.col[k] = blue[k];
					v2.col[k] = blue[k];
					v3.col[k] = blue[k];
					v4.col[k] = blue[k];
				}
			}

			vertices.push_back(v1);
			vertices.push_back(v2);
			vertices.push_back(v3);

			vertices.push_back(v2);
			vertices.push_back(v3);
			vertices.push_back(v4);
		}
	}

	glewInit();
	// Step 1: Generate and Bind a VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Step 2: Generate and Bind a VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Step 3: Fill the VBO with Data
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Step 4: Set Up Vertex Attribute Pointers
	// Position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0);

	// Colour attribute
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));
	//glEnableVertexAttribArray(1);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, col));
	glEnableClientState(GL_COLOR_ARRAY);

	// Texture attribute
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);				//Remove from Active
	glBindBuffer(GL_ARRAY_BUFFER, 0);	//Remove from Active

	setupComplete = true;
}


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
	glTranslatef(0.0f, 0.0f, 10.0f);	//Sets polygons 10 units above the pcb
	arGetDebugMode(arHandle, &debugMode);


	if (simulationStarted && setupComplete)
	{
		glBindTexture(GL_TEXTURE_1D, textureID);
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

	if (setupComplete)
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		//glEnableClientState(GL_VERTEX_ARRAY);
		//glVertexPointer(2, GL_FLOAT, sizeof(Vertex), vertices.data()->pos);

		//glEnableClientState(GL_COLOR_ARRAY);
		//glColorPointer(3, GL_FLOAT, sizeof(Vertex), vertices.data()->tex);

		//glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		//glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY);

		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_1D, textureID);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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