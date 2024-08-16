// simple Point class that holds x, y coordinates
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

    Point(float xCoord, float yCoord)
    {
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