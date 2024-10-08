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
    double x, y;
    std::optional<float> z;

    RGBA color = RGBA(255.0f, 255.0f, 255.0f);

    float WINDOW_WIDTH = 900.0f;
    float WINDOW_HEIGHT = 900.0f;
    float Z_MAX = 900.0f;
    float Z_MIN = 0.0f;

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

    Point operator-(const Point &other) const
    {
        return Point(x - other.x, y - other.y, z.value_or(0) - other.z.value_or(0));
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

    matrix getNormalizedVector()
    {
        matrix pointVector = z.has_value() ? matrix(3, 1) : matrix(2, 1);

        pointVector.addElement(0, 0, normalizeCoordinate(x, 0, WINDOW_WIDTH));
        pointVector.addElement(1, 0, normalizeCoordinate(y, 0, WINDOW_HEIGHT));
        if (z.has_value())
        {
            pointVector.addElement(2, 0, normalizeCoordinate(z.value(), 0, Z_MAX));
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

    Point crossProduct(Point otherDirectionVectorPoint)
    {
        double newX = y * otherDirectionVectorPoint.z.value() - z.value() * otherDirectionVectorPoint.y;
        double newY = z.value() * otherDirectionVectorPoint.x - x * otherDirectionVectorPoint.z.value();
        double newZ = x * otherDirectionVectorPoint.y - y * otherDirectionVectorPoint.x;
        return Point(newX, newY, newZ);
    }

    float dotProduct(Point otherPointVector)
    {
        return x * otherPointVector.x + y * otherPointVector.y + z.value_or(0) * otherPointVector.z.value_or(0);
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

    float normalizeCoordinate(float coord, float minCoord, float maxCoord)
    {
        return (coord - minCoord) / (maxCoord - minCoord) * 2.0f - 1.0f;
    }

    /*
    Normalizes point from screen space into world space (-1.0, 1.0) range
    */
    void normalize()
    {
        x = normalizeCoordinate(x, 0, WINDOW_WIDTH);
        y = normalizeCoordinate(y, 0, WINDOW_HEIGHT);
        z = normalizeCoordinate(z.value(), Z_MIN, Z_MAX);
    }

    /*
    Normalize Point between (0, 1)
    */
    Point stdNormalize()
    {
        double magnitude = sqrt(x * x + y * y + z.value() * z.value());
        if (magnitude > 0)
        {
            float newX = x / magnitude;
            float newY = y / magnitude;
            float newZ = z.value() / magnitude;
            return Point(newX, newY, newZ);
        }
        else
        {
            return Point(0, 0, 0);
        }
    }

    void projectPerspective(float FOV)
    {
        // need to add focal length for this to work properly (for now just mock)
        float fov_radians = FOV * M_PI / 180.0f;

        // Calculate focal length ()
        float focalLength = (WINDOW_WIDTH / 2.0f) / std::tan(fov_radians / 2.0f);

        if (z.has_value())
        {
            // Normalize the coordinates first (putting them into world space)
            float normalizedX = normalizeCoordinate(x, 0, WINDOW_WIDTH);
            float normalizedY = normalizeCoordinate(y, 0, WINDOW_HEIGHT);
            float normalizedZ = normalizeCoordinate(z.value(), Z_MIN, Z_MAX);

            // Calculate the scaling factor based on the distance from the camera (z-value)
            // WOW THE CULLING ISSUE WAS HERE BC I WAS ADDING THE UN-NORMALIZED Z TO THE DENOMINATOR
            float scale = focalLength / (focalLength + z.value());

            // Debug output before projection
            // std::cout << "Original (x, y, z): (" << x << ", " << y << ", " << z.value() << ")" << std::endl;

            // scale coordinates according to projection scale
            normalizedX *= scale;
            normalizedY *= scale;

            // map projected coordinates back to screen space
            x = (normalizedX + 1.0f) * WINDOW_WIDTH / 2.0f;
            y = (1.0f - normalizedY) * WINDOW_HEIGHT / 2.0f;

            // Debug output after projection
            // std::cout << "Projected (x, y): (" << x << ", " << y << ")" << std::endl;
            // std::cout << "Projected normalized (x,y): (" << normalizeCoordinate(x, 0, WINDOW_WIDTH) << "," << normalizeCoordinate(y, 0, WINDOW_HEIGHT) << ")" << std::endl;
        }
    }

    Point getProjected(float FOV)
    {
        // need to add focal length for this to work properly (for now just mock)
        float fov_radians = FOV * M_PI / 180.0f;

        // Calculate focal length ()
        float focalLength = (WINDOW_WIDTH / 2.0f) / std::tan(fov_radians / 2.0f);

        if (z.has_value())
        {
            // Normalize the coordinates first (putting them into world space)
            float normalizedX = normalizeCoordinate(x, 0, WINDOW_WIDTH);
            float normalizedY = normalizeCoordinate(y, 0, WINDOW_HEIGHT);
            float normalizedZ = normalizeCoordinate(z.value(), Z_MIN, Z_MAX);

            float scale = focalLength / (focalLength + z.value());

            // scale coordinates according to projection scale
            normalizedX *= scale;
            normalizedY *= scale;

            // map projected coordinates back to screen space
            normalizedX = (normalizedX + 1.0f) * WINDOW_WIDTH / 2.0f;
            normalizedY = (1.0f - normalizedY) * WINDOW_HEIGHT / 2.0f;

            return Point(normalizedX, normalizedY);
        }
        return Point(0, 0);
    }

    void print()
    {
        if (z)
        {
            std::cout << "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z.value_or(1)) + ")\n";
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