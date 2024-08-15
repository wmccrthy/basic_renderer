#include <vector>
#include "point.h"
#include <numeric>
#include "iostream"
#pragma once

// basic line class; for our purposes line should have:
//   two points
//   methods for computing values relevant to drawing the line (angle, slope, ...)
class Line
{
public:
    Point point1;
    Point point2;
    std::vector<Point> pointsToDraw;

    Line(float x1, float y1, float x2, float y2)
    {
        point1 = Point(x1, y1);
        point2 = Point(x2, y2);
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

    /*Returns a vector of points at which we want to render pixels in order to draw the line from (x1, y1) to (x2, y2).
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
        return nextPoint;
        // if (point2.x > point1.x) {
        //     nextPoint.x = point1.x + std::cos(angle) * iter;
        // } else {
        //     nextPoint.x = point1.x - std::cos(angle) * iter;
        // }
        // if (point2.y > point1.y) {
        //     nextPoint.y = point1.y + std::sin(angle) * iter;
        // } else {
        //     nextPoint.y = point1.y - std::sin(angle) * iter;
        // }
        // return nextPoint;
    }
};