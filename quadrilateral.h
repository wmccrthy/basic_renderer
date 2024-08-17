#include <vector>
#include "iostream"
#include "line.h"
#include "point.h"
#include "math.h"
#include "color.h"
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
    RGBA color = RGBA(255, 255, 255);

    Quadrilateral()
    {
    }

    /*
    Construct Quadrilateral given four points. Use given points to instantiate the quadrilateral's four lines.
    */
    Quadrilateral(Point p1, Point p2, Point p3, Point p4, RGBA c = RGBA(255, 255, 255))
    {
        // this->findCorners(p1, p2, p3, p4);
        printf("quad constructing");
        c.print();
        color = c;
        this->fillLines(p1, p2, p3, p4);
        this->getPointsToDraw();
    }

    void setColor(std::vector<Point> points)
    {
        for (auto &p : points)
        {
            p.color = color;
        }
    }

    void fillLines(Point p1, Point p2, Point p3, Point p4)
    {
        lines[0] = Line(p1, p2, color);
        lines[1] = Line(p2, p3, color);
        lines[2] = Line(p3, p4, color);
        lines[3] = Line(p4, p1, color);
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

    Rectangle(Point p1, Point p2, Point p3, Point p4, RGBA c = RGBA(255, 255, 255), bool needsForm = true)
    {
        color = c;
        if (needsForm)
        {
            // properly assign passed points to their corners (aligns as rectangle)
            this->findCorners(p1, p2, p3, p4);
        }
        // then fill our lines arr
        this->fillLines(p1, p2, p3, p4);
        this->getPointsToDraw();
    }

    /*
   When constructing Rectangle, we want to enter p1-p4 as the following:
       - p1 = top left
       - p2 = top right
       - p3 = bottom right
       - p4 = bottom left
   This method essentially converts the Quadrilateral formed by four points to rectangular form
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
        // TO-DO: implement
    }

    /*
    Constructor for Square that can take arbitrary four points and automatically adjusts them to be square(ish)
    Parameters:
        - Point p1, ..., p4: points that form square (can be arbitrary)
        - bool needsForm: boolean to control whether points are reformed
    */
    Square(Point p1, Point p2, Point p3, Point p4, RGBA c = RGBA(255, 255, 255), bool needsForm = true)
    {
        color = c;
        // properly assign passed points to their corners
        if (needsForm)
        {
            this->findCorners(p1, p2, p3, p4);
            // first align the four points
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