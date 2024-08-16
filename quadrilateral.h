#include <vector>
#include "iostream"
#include "line.h"
#include "point.h"
#include "math.h"
#pragma once

/*
Quadrilateral has:
    - four points (p1, p2, p3, p4)
    - four lines connecting four points:
        - l1(p1, p2)
        - l2(p2, p3)
        - l3(p3, p4)
        - l4(p4, p1)
*/
class Quadrilateral
{
public:
    Line lines[4];
    std::vector<Point> pointsToDraw;

    Quadrilateral()
    {
    }

    /*
    Construct Quadrilateral given four points. Use given points to instantiate the quadrilateral's four lines.
    */
    Quadrilateral(Point p1, Point p2, Point p3, Point p4)
    {
        this->fillLines(p1, p2, p3, p4);
        this->getPointsToDraw();
    }

    void fillLines(Point p1, Point p2, Point p3, Point p4)
    {
        lines[0] = Line(p1, p2);
        lines[1] = Line(p2, p3);
        lines[2] = Line(p3, p4);
        lines[3] = Line(p4, p1);
    }

    void getPointsToDraw()
    {
        for (auto &l : this->lines)
        {
            // std::cout << "line length" + std::to_string(l.getLength()) + "\n";
            for (auto &p : l.pointsToDraw)
            { // i'd imagine there can/may be redundant lines if we do this??? best practice to use a set to avoid dupes
                pointsToDraw.emplace_back(p);
            }
        }
    }

    /*
    Comparator functions for sorting points by X
    */
    static bool compareByX(Point &a, Point &b)
    {
        return a.x < b.x;
    }
    /*
    Comparator functions for sorting points by Y
    */
    static bool compareByY(Point &a, Point &b)
    {
        return a.y < b.y;
    }

    /*
    When constructing Quadrilateral, we want to enter p1-p4 as the following: p1 = top left, p2 = top right, p3 = bottom right, p4 = bottom left
    This method essentially checks to ensure the points are organized as such and if not, reassigns them according to the above
    */
    void findCorners(Point &topLeft, Point &topRight, Point &bottomRight, Point &bottomLeft)
    {
        std::vector<Point> points = {topLeft, topRight, bottomRight, bottomLeft};
        std::vector<Point> sortedByX = points;
        std::vector<Point> sortedByY = points;

        // Sort points by x and y
        std::sort(sortedByX.begin(), sortedByX.end(), compareByX);
        std::sort(sortedByY.begin(), sortedByY.end(), compareByY);

        // Top left has the smallest x and smallest y
        topLeft = Point(sortedByX[0].x, sortedByY[0].y);

        // Top right has the largest x and smallest y
        topRight = Point(sortedByX[3].x, sortedByY[0].y);

        // Bottom right has the largest x and largest y
        bottomRight = Point(sortedByX[3].x, sortedByY[3].y);

        // Bottom left has the smallest x and largest y
        bottomLeft = Point(sortedByX[0].x, sortedByY[3].y);
    }
};

/*
Basic Rectangle class that inherits from Quadrilateral
Distinction is that Rectangle constructor does the following:
    - Takes in four Points (p1, p2, p3, p4, can be random/arbitrary)
    - Aligns p1 with p2 on the Y axis
    - Aligns p2 with p3 on the X axis
    - Aligns p3 with p4 on the Y axis
    - Aligns p4 with p1 on the X axis
    - Let align =
*/
class Rectangle : public Quadrilateral
{
public:
    Rectangle()
    {
    }

    Rectangle(Point p1, Point p2, Point p3, Point p4, bool needsForm = true)
    {
        if (needsForm)
        {
            // properly assign passed points to their corners
            this->findCorners(p1, p2, p3, p4);
            // first align the four points
            this->alignPoints(p1, p2, p3, p4);
        }
        // then fill our lines arr
        this->fillLines(p1, p2, p3, p4);
        this->getPointsToDraw();
    }

    float alignedX(float x1, float x2)
    {
        return floor((x1 + x2) / 2);
    }

    float alignedY(float y1, float y2)
    {
        return floor((y1 + y2) / 2);
    }

    /*
    Method that returns a pointer to array of points that have been aligned s.t they form a Rectangle when connected.
    */
    Point *getAlignedPoints(Point p1, Point p2, Point p3, Point p4)
    {
        // align p1 and p2 on the y axis
        float alignedY = this->alignedY(p1.y, p2.y);
        p1.y = alignedY;
        p2.y = alignedY;

        // align p2 and p3 on the x axis
        float alignedX = this->alignedX(p2.x, p3.x);
        p2.x = alignedX;
        p3.x = alignedX;

        // align p3 and p4 on the y axis
        alignedY = this->alignedY(p3.y, p4.y);
        p3.y = alignedY;
        p4.y = alignedY;

        // align p4 and p1 on the x axis
        alignedX = this->alignedX(p4.x, p1.x);
        p4.x = alignedX;
        p1.x = alignedX;

        Point alignedPoints[4] = {p1, p2, p3, p4};
        return alignedPoints;
    }

    void alignPoints(Point &p1, Point &p2, Point &p3, Point &p4)
    {
        Point *alignedPoints = this->getAlignedPoints(p1, p2, p3, p4);
        p1 = alignedPoints[0];
        p2 = alignedPoints[1];
        p3 = alignedPoints[2];
        p4 = alignedPoints[3];
    }
};

/*
Basic Square class that inherits from Rectangle
Distinction is that Square constructor adjusts points s.t all side lengths are even
*/
class Square : public Rectangle
{
public:
    /*
    Constructs Square given top left point and length
    More performant
    Parameters:
        - Point topLeft: top left of square (essentially anchor point)
        - float length: side length of square
    */
    Square(Point topLeft, float length)
    {
        // to-do: implement
    }

    /*
    Constructor for Square that can take arbitrary four points and automatically adjusts them to be square(ish)
    Parameters:
        - Point p1, ..., p4: points that form square (can be arbitrary)
        - bool needsForm: boolean to control whether points are reformed
    */
    Square(Point p1, Point p2, Point p3, Point p4, bool needsForm = true)
    {
        // properly assign passed points to their corners
        if (needsForm)
        {
            this->findCorners(p1, p2, p3, p4);
            // first align the four points
            this->alignPoints(p1, p2, p3, p4);
            this->fillLines(p1, p2, p3, p4);
            // make side lengths even
            this->alignXYSize(p1, p2, p3, p4);
        }
        // then fill our lines arr
        this->fillLines(p1, p2, p3, p4);

        this->getPointsToDraw();
    }

    /*
    Function used to set all square line lengths as the same
    */
    void alignXYSize(Point &p1, Point &p2, Point &p3, Point &p4)
    {
        // easy way to get length would be to average length of each line (aligned line)
        float lineMin = 9999;
        float lineAvg = 0;
        for (auto &l : this->lines)
        {
            lineAvg += floor(l.getLength() / 4);
        }
        this->adjustPointsForNewLength(p1, p2, p3, p4, lineAvg);
    }

    /*
    Takes in four points and adjusts them s.t they form a square when connected
    */
    void adjustPointsForNewLength(Point &p1, Point &p2, Point &p3, Point &p4, float length)
    {

        // p1 -> p2 (diff on x axis)
        float dx = abs(p2.x - p1.x), dy = abs(p2.y - p1.y);
        float dXAvg = length - dx, dYAvg = length - dy;
        p1.x -= floor(dXAvg / 2); // top left
        p2.x += floor(dXAvg / 2); // top right
        // p2 -> p3 (diff on y axis)
        dy = abs(p2.y - p3.y);
        dYAvg = length - dy;
        p2.y -= floor(dYAvg / 2); // top right
        p3.y += floor(dYAvg / 2); // bottom right
        // p3 -> p4 (diff on x axis)
        dx = abs(p3.x - p4.x);
        dXAvg = length - dx;
        p4.x -= floor(dXAvg / 2); // bottom left
        p3.x += floor(dXAvg / 2); // bottom right
        // p4 -> p1 (diff on y axis)
        dy = abs(p4.y - p1.y);
        dYAvg = length - dy;
        p1.y -= floor(dYAvg / 2); // top left
        p4.y += floor(dYAvg / 2); // bottom left
    }
};
