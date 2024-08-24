// simple Point class that holds x, y coordinates
#include "color.h"
#include "matmul.h"
#pragma once

/*
Basic Point class (supports 2D and 3D points)
Point has:
    - x coordinate
    - y coordinate
    - z coordinate (optional)
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

    Point(float xCoord, float yCoord, float zCoord, RGBA c = RGBA(255.0f, 255.0f, 255.0f))
    {
        color = c;
        x = xCoord;
        y = yCoord;
        z = zCoord;
    }

    matrix getVector(bool isAffine = false)
    {
        matrix pointVector;
        if (isAffine)
        {
            pointVector = matrix(1, 4);
            pointVector.addElement(3, 0, 1); // affine point
        }
        else
        {
            pointVector = matrix(1, 3);
        }
        pointVector.addElement(0, 0, x);
        pointVector.addElement(0, 1, y);
        if (z)
        {
            pointVector.addElement(0, 2, z);
        }
        return pointVector;
    }

    void print()
    {
        if (z)
        {
            std::cout << "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
        }
        else
        {
            std::cout << "(" + std::to_string(x) + "," + std::to_string(y) + ")";
        }
    }

    Point()
    {
    }
};