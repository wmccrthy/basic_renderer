// simple Point class that holds x, y coordinates
#pragma once

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