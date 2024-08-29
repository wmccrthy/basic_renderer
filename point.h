// simple Point class that holds x, y coordinates
#include "color.h"
#include <optional>
#include "math.h"
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
    float x, y;
    std::optional<float> z;

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
            pointVector = z.has_value() ? matrix(4, 1) : matrix(3, 1);
            pointVector.addElement(pointVector.numRows - 1, 0, 1); // affine point
        }
        else
        {
            pointVector = z.has_value() ? matrix(3, 1) : matrix(2, 1);
        }
        pointVector.addElement(0, 0, x);
        pointVector.addElement(1, 0, y);
        if (z.has_value())
        {
            pointVector.addElement(2, 0, z.value());
        }
        return pointVector;
    }

    // method expects more rows than cols (column vector) as point
    // if more cols than rows, will take transpose
    Point getPointFromVector(matrix vector)
    {
        int r = vector.numRows, c = vector.numCols;
        if (c > r)
        {
            vector = vector.transpose();
        }
        Point convertedPoint = vector.numRows > 3 ? Point(vector[0][0], vector[1][0], vector[2][0]) : Point(vector[0][0], vector[1][0]);
        return convertedPoint;
    }

    void translate(float xTrans, float yTrans, float zTrans)
    {
        x += xTrans;
        y += yTrans;
        if (z.has_value())
        {
            z = z.value() + zTrans;
        }
    }

    void print()
    {
        if (z)
        {
            std::cout << "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(*z) + ")\n";
        }
        else
        {
            std::cout << "(" + std::to_string(x) + "," + std::to_string(y) + ")\n";
        }
    }

    Point()
    {
    }
};