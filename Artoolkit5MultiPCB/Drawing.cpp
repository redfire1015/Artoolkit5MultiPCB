#include "Headers/Drawing.h"



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

	//std::cout << glGetError() << std::endl;
	// 
	//float lineWidth[2];
	//glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);
	//gets Supported widths and returns into  array - Not in this case we are told 0.5-10 on GTX1060 on windows 11

	//Print information about segments and polygons
	for (size_t i = 0; i < loadedPCB.getNumberOfLayers(); ++i)
	{
		const Layer& currentLayer = loadedPCB.getPCBLayers()[i];
		// Print information about segments
		for (int j = 0; j < currentLayer.getNumberOfLayerSegments(); ++j)
		{
			const Segment& currentSegment = currentLayer.getLayerSegments()[j];
			//currentSegment.getSegmentThickness()
			if (currentLayer.getLayerName() == "F.Cu") {
				glColor3f(1.0, 0.0, 0.0); // Set color to red
			}
			if (currentLayer.getLayerName() == "B.Cu") {
				glColor3f(0.0, 0.0, 1.0); // Set color to red
			}
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

			// Draw the rectangle using GL_TRIANGLES
			glBegin(GL_TRIANGLES);
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