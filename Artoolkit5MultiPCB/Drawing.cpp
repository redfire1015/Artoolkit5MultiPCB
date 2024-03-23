#include "Headers/Drawing.h"

GLuint textureID;
GLuint VAO;		//Vertex Array Object ID
GLuint vbo;		//Vertex Buffer object

std::vector<Vertex> vertices;

bool setupComplete = false;

spMATRIX vertexToTextureRelationship;
double* vertexToTextureRelationship_aa;
ML_INT* vertexToTextureRelationship_ja;
ML_INT* vertexToTextureRelationship_ia;

//TODO: Function that loads the PCB segments once

void loadVertexToGPU() {

	//Load Segments into Array
	for (size_t i = 0; i < loadedPCB.getNumberOfLayers(); ++i)
	{
		const Layer& currentLayer = loadedPCB.getPCBLayers()[i];

		for (int j = 0; j < currentLayer.getNumberOfLayerSegments(); ++j)
		{
			const Segment& currentSegment = currentLayer.getLayerSegments()[j];

			//setColoursAndTextures(currentLayer, currentSegment);

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

	//Temp Code For Generating VAO Matirx
	int relationshipMatrix[666 * 36] = { 0 };

	for (size_t i = 0; i < loadedPCB.getNumberOfLayers(); ++i)
	{
		const Layer& currentLayer = loadedPCB.getPCBLayers()[i];

		for (int j = 0; j < currentLayer.getNumberOfLayerSegments(); ++j)
		{
			const Segment& currentSegment = currentLayer.getLayerSegments()[j];

			switch (currentSegment.getSegmentNet())
			{
			case 20:
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (0 * 36) + 2] = 1;	//C1
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (1 * 36) + 2] = 1;	//C1
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (2 * 36) + 2] = 1;	//C1
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (3 * 36) + 2] = 1;	//C1
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (4 * 36) + 2] = 1;	//C1
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (5 * 36) + 2] = 1;	//C1
				break;
			case 21:
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (0 * 36) + 7] = 1;	//C2
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (1 * 36) + 7] = 1;	//C2
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (2 * 36) + 7] = 1;	//C2
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (3 * 36) + 7] = 1;	//C2
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (4 * 36) + 7] = 1;	//C2
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (5 * 36) + 7] = 1;	//C2
				break;
			case 2:
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (0 * 36) + 8] = 1;	//C3
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (1 * 36) + 8] = 1;	//C3
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (2 * 36) + 8] = 1;	//C3
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (3 * 36) + 8] = 1;	//C3
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (4 * 36) + 8] = 1;	//C3
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (5 * 36) + 8] = 1;	//C3
				break;
			case 3:
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (0 * 36) + 15] = 1;	//C4
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (1 * 36) + 15] = 1;	//C4
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (2 * 36) + 15] = 1;	//C4
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (3 * 36) + 15] = 1;	//C4
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (4 * 36) + 15] = 1;	//C4
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (5 * 36) + 15] = 1;	//C4
				break;
			case 4:
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (0 * 36) + 16] = 1;	//C5
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (1 * 36) + 16] = 1;	//C5
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (2 * 36) + 16] = 1;	//C5
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (3 * 36) + 16] = 1;	//C5
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (4 * 36) + 16] = 1;	//C5
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (5 * 36) + 16] = 1;	//C5
				break;
			case 5:
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (0 * 36) + 17] = 1;	//C6
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (1 * 36) + 17] = 1;	//C6
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (2 * 36) + 17] = 1;	//C6
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (3 * 36) + 17] = 1;	//C6
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (4 * 36) + 17] = 1;	//C6
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (5 * 36) + 17] = 1;	//C6
				break;
			case 6:
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (0 * 36) + 30] = 1;	//C7
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (1 * 36) + 30] = 1;	//C7
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (2 * 36) + 30] = 1;	//C7
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (3 * 36) + 30] = 1;	//C7
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (4 * 36) + 30] = 1;	//C7
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (5 * 36) + 30] = 1;	//C7
				break;
			case 7:
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (0 * 36) + 31] = 1;	//C8
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (1 * 36) + 31] = 1;	//C8
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (2 * 36) + 31] = 1;	//C8
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (3 * 36) + 31] = 1;	//C8
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (4 * 36) + 31] = 1;	//C8
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (5 * 36) + 31] = 1;	//C8
				break;
			case 8:
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (0 * 36) + 32] = 1;	//C9
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (1 * 36) + 32] = 1;	//C9
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (2 * 36) + 32] = 1;	//C9
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (3 * 36) + 32] = 1;	//C9
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (4 * 36) + 32] = 1;	//C9
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (5 * 36) + 32] = 1;	//C9
				break;
			case 9:
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (0 * 36) + 33] = 1;	//C10
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (1 * 36) + 33] = 1;	//C10
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (2 * 36) + 33] = 1;	//C10
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (3 * 36) + 33] = 1;	//C10
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (4 * 36) + 33] = 1;	//C10
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (5 * 36) + 33] = 1;	//C10
				break;
			case 10:
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (0 * 36) + 34] = 1;	//C11
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (1 * 36) + 34] = 1;	//C11
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (2 * 36) + 34] = 1;	//C11
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (3 * 36) + 34] = 1;	//C11
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (4 * 36) + 34] = 1;	//C11
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (5 * 36) + 34] = 1;	//C11
				break;
			case 11:
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (0 * 36) + 35] = 1;	//C12
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (1 * 36) + 35] = 1;	//C12
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (2 * 36) + 35] = 1;	//C12
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (3 * 36) + 35] = 1;	//C12
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (4 * 36) + 35] = 1;	//C12
				relationshipMatrix[(i * 51 * 6 * 36) + (j * 6 * 36) + (5 * 36) + 35] = 1;	//C12
				break;
			default:
				break;
			}

		}
	}

	//for (int i = 0; i < 666; i++)
	//{
	//	for (int j = 0; j < 36; j++)
	//	{
	//		if (j != 35)
	//		{
	//			std::cout << relationshipMatrix[(i * 36) + j] << ",";
	//		}
	//		else
	//		{
	//			std::cout << relationshipMatrix[(i * 36) + j] << ";";
	//		}
	//	}
	//	std::cout << std::endl;
	//}

	vertexToTextureRelationship.nr = 666;
	vertexToTextureRelationship.nc = 36;

	double test_AA[] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, };
	ML_INT test_jA[] = { 8,8,8,8,8,8,15,15,15,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16,16,16,17,17,17,17,17,17,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,2,2,2,2,2,2,7,7,7,7,7,7,7,7,7,7,7,7, };
	ML_INT test_iA[] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228,228, };

	vertexToTextureRelationship.ia = test_iA;
	vertexToTextureRelationship.ja = test_jA;
	vertexToTextureRelationship.aa = test_AA;

	//printSmallSparse(vertexToTextureRelationship);

	double* testOut = new double[666];
	vecZero(testOut, 666);

	double* testSolMat = new double[36];

	for (int m = 0; m < 36; m++)
	{
		testSolMat[m] = m;
	}

	spMatVec(&vertexToTextureRelationship, testSolMat, testOut);

	//for (int m = 0; m < 666; m++)
	//{
	//	std::cout << testOut[m] << std::endl;
	//}

	//End Temp Code for Generating VAO Matrix

	glewInit();
	// Step 1: Generate and Bind a VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Step 2: Generate and Bind a VBO
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
	glTexCoordPointer(1, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, tex));
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
	//glEnableVertexAttribArray(2);

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

	if (setupComplete)
	{
		glBindTexture(GL_TEXTURE_1D, 0);	//Not using Textures for basic Display

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glBindVertexArray(0);				//Remove from Active
		glBindBuffer(GL_ARRAY_BUFFER, 0);	//Remove from Active

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

			double* texCoordsOut = new double[666];
			vecZero(texCoordsOut, 666);
			spMatVec(&vertexToTextureRelationship, transientCurrentSolution, texCoordsOut);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			for (size_t i = 0; i < vertices.size(); i++) {
				// Accessing pos array inside the current vertex
				//vertices[i].tex[0] = texCoordsOut[i] / readSettings.getColourMapMax(); // Accessing the texture

				//Scaling the texture Coord for each Vertex
				float textureCoord = max(min(texCoordsOut[i], readSettings.getColourMapMax()), readSettings.getColourMapMin());
				float scaled = (textureCoord - readSettings.getColourMapMin()) / (readSettings.getColourMapMax() - readSettings.getColourMapMin());

				//Writing Texture Coord to Vertex Buffer
				vertices[i].tex[0] = scaled;
			}

			//Fill the VBO with New Data
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
			glBindTexture(GL_TEXTURE_1D, textureID); //Enable Textures for Use in Simulation

			glBindVertexArray(0);				//Remove from Active
			glBindBuffer(GL_ARRAY_BUFFER, 0);	//Remove from Active
		}

		//glClearColor(0.0, 0.0, 0.0, 0.0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		glBindVertexArray(0);
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