#include "Headers/XYCoord.h"

// Default constructor
XYCoord::XYCoord() : x(0.0), y(0.0)
    {
    }

    // Overloaded constructor with default values
XYCoord::XYCoord(float x_in, float y_in) : x(x_in), y(y_in)
    {
    }

    void XYCoord::setXYCoord(float x_in, float y_in)
    {
        x = x_in;
        y = y_in;
    }

    XYCoord::XYCoord(const XYCoord& other) // Copy Constructor
        : x(other.x), y(other.y)
    {
    }

    XYCoord& XYCoord::operator=(const XYCoord& other) // Assignment constructor
    {
        if (this != &other) // self-assignment check
        {
            x = other.x;
            y = other.y;
        }
        return *this;
    }

    float XYCoord::getXCoord() const
    {
        return (x);
    }

    float XYCoord::getYCoord() const
    {
        return (y);
    }

    XYCoord::~XYCoord()
    {
    }

