// simple Point class that holds x, y coordinates
#include "color.h"
#pragma once

/*
Basic Point class
Point has:
    - x coordinate
    - y coordinate
*/
struct Point
{
public:
    float x, y, z;
    RGBA color = RGBA(255.0f, 255.0f, 255.0f);

    Point(float xCoord, float yCoord, RGBA c = RGBA(255.0f, 255.0f, 255.0f))
    {
        color = c;
        x = xCoord;
        y = yCoord;
    }

    Point()
    {
    }
};

/*
3D Point class
Point3D has:
    - x coord
    - y coord
    - z coord
*/
struct Point3D
{
    float x, y, z;
    // TO-DO: implement
};