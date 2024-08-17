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
        printf("constructing line");
        c.print();
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
        pointsToDraw.emplace_back(point1);
        // debugging output
        // std::cout << "angle: " + std::to_string(angle) + "\n";
        // std::cout << "point1: " + std::to_string(point1.x) + "," + std::to_string(point1.y) + "\n";

        // we want to draw from point1 to point 2
        for (int i = 0; i < lineLength; i++)
        {
            Point nextPoint = this->getNextPointOnLine(angle, i);
            float x = nextPoint.x;
            float y = nextPoint.y;
            pointsToDraw.emplace_back(nextPoint);
            // debugging output
            // std::cout << "point" + std::to_string(i + 2) + ": " + std::to_string(x) + "," + std::to_string(y) + "\n";
        }
        pointsToDraw.emplace_back(point2);
        // debugging output
        // std::cout << "last point: " + std::to_string(point2.x) + "," + std::to_string(point2.y);
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
};