#ifndef XYCOORD
#define XYCOORD

class XYCoord
{
public:
	// Default constructor
	XYCoord();

	// Overloaded constructor with default values
	XYCoord(float x_in, float y_in);

	void setXYCoord(float x_in, float y_in);

	XYCoord(const XYCoord& other); // Copy Constructor

	XYCoord& operator=(const XYCoord& other); // Assignment constructor

	float getXCoord() const;

	float getYCoord() const;

	~XYCoord();

private:
	float x;
	float y;
};

#endif