#include <vector>
#include "point.h"
#include <numeric>
#include "iostream"
#include "color.h"
#pragma once

/*
Basic Line class
Line has:
    - two points (point1 and point2)
    - vector of points to draw (every pixel between point1 and point2) that we need to render to actually "draw" the line
*/
class Line
{
public:
    Point point1;
    Point point2;
    std::vector<Point> pointsToDraw;
    RGBA color = RGBA(255, 255, 255);

    Line()
    {
    }

    Line(float x1, float y1, float x2, float y2, RGBA c = RGBA(255, 255, 255))
    {
        color = c;
        point1 = Point(x1, y1, color);
        point2 = Point(x2, y2, color);
        this->getPointsToDraw();
    }

    Line(Point p1, Point p2, RGBA c = RGBA(255, 255, 255))
    {
        point1 = p1;
        point2 = p2;
        point1.color = c, point2.color = c;
        // printf("constructing line");
        // c.print();
        color = c;
        this->getPointsToDraw();
    }

    // Return the slope between our line's two points
    float getSlope()
    {
        float dx = point2.x - point1.x;
        float dy = point2.y - point1.y;
        // slope is just rise over run
        return dy / dx;
    }

    // Return the direction vector of the line (when it is bw two 3d points)
    Point getDirectionVector()
    {
        return Point(point2.x - point1.x, point2.y - point1.y, point2.z.value_or(1) - point1.z.value_or(1));
    }

    /* use pythag theorem to get line length
    we use:
         - difference in x1 and x2 (call this side a)
         - difference in y1 and y2 coord (call this side b)
    to find the length of the line from (x1, y1) to (x2, y2)
    */
    float getLength()
    {
        float dx = point2.x - point1.x;
        float dy = point2.y - point1.y;
        if (point1.z.has_value() && point2.z.has_value())
        {
            float dz = point2.z.value() - point1.z.value();
            return std::sqrt(dx * dx + dy * dy + dz * dz);
        }
        return std::sqrt(dx * dx + dy * dy);
    }

    /* returns arc tan of slope, essentially providing the angle at which we want to draw the line
    (angle between point1 and point2) */
    float getArcTan()
    {
        float dx = point2.x - point1.x;
        float dy = point2.y - point1.y;
        return std::atan2(dy, dx); // need to use arctan2 bc it takes +/- into account s.t output is accurate
    }

    /*Compiles a vector of points at which we want to render pixels in order to draw the line from (x1, y1) to (x2, y2).
    Computes points using the line's length and the angle between p1 and p2*/
    void getPointsToDraw()
    {
        float lineLength = this->getLength();
        float angle = this->getArcTan();
        Point directionVector = this->getDirectionVector();

        pointsToDraw.emplace_back(point1);

        // we want to draw from point1 to point 2
        for (int i = 1; i < lineLength; i++)
        {
            Point nextPoint;
            if (point1.z.has_value() && point2.z.has_value())
            {
                nextPoint = this->getNextPointOnLine3D(directionVector, i);
            }
            else
            {
                nextPoint = this->getNextPointOnLine(angle, i);
                float x = nextPoint.x;
                float y = nextPoint.y;
            }
            pointsToDraw.emplace_back(nextPoint);
        }
        pointsToDraw.emplace_back(point2);
    }

    Point getNextPointOnLine(float angle, int iter)
    {
        Point nextPoint;
        nextPoint.x = point1.x + std::cos(angle) * iter;
        nextPoint.y = point1.y + std::sin(angle) * iter;
        nextPoint.color = color;
        // std::cout << "getting next line point: ";
        // color.print();

        /*
        Why couldn't we have just used y = mx + b for this??
        */
        return nextPoint;
    }

    // Calculate the next point along the line in 3D space
    Point getNextPointOnLine3D(Point direction, int iter)
    {
        Point nextPoint;
        nextPoint.x = point1.x + direction.x * iter / getLength();
        nextPoint.y = point1.y + direction.y * iter / getLength();
        nextPoint.z = point1.z.value_or(1) + direction.z.value_or(1) * iter / getLength();
        nextPoint.color = color;

        return nextPoint;
    }
};