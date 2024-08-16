// simple Point class that holds x, y coordinates
#pragma once

/*
Basic Point class
Point has:
    - x coordinate
    - y coordinate
*/
class Point
{
public:
    float x;
    float y;

    Point(float xCoord, float yCoord)
    {
        x = xCoord;
        y = yCoord;
    }

    Point()
    {
    }
};