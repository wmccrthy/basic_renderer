#include <vector>
#include "iostream"
#include "line.h"
#include "point.h"
#include "math.h"
#include "color.h"
#pragma once

/*
Super lightweight Shape class to be inherited by all other shape classes
Only real purpose is so that we can have vector of shapes we want to draw in Screen class
*/
class Shape
{
public:
    std::vector<Point> pointsToDraw;
    Shape()
    {
    }

    virtual ~Shape()
    {
    }

    virtual void getPointsToDraw() = 0;

    void print()
    {
        std::cout << "shape\n";
    }

};

/*
Triangle has:
    - three points (define in clockwise order)
    - three lines
*/
class Triangle : public Shape
{
public:
    Line lines[3];
    RGBA color = RGBA(255, 255, 255);

    Triangle()
    {
    }

    Triangle(Point p1, Point p2, Point p3, RGBA c = RGBA(255, 255, 255))
    {
        color = c;
        this->fillLines(p1, p2, p3);
        this->getPointsToDraw();
    }

    void fillLines(Point p1, Point p2, Point p3)
    {
        lines[0] = Line(p1, p2);
        lines[1] = Line(p2, p3);
        lines[2] = Line(p3, p1);
    }

    void getPointsToDraw() override
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
};


/*
Quadrilateral has:
    - four points (p1, p2, p3, p4)
    - four lines connecting four points:
        - l1(p1, p2)
        - l2(p2, p3)
        - l3(p3, p4)
        - l4(p4, p1)
*/
class Quadrilateral : public Shape
{
public:
    Line lines[4];
    RGBA color = RGBA(255, 255, 255);

    Quadrilateral()
    {
    }

    /*
    Construct Quadrilateral given four points. Use given points to instantiate the quadrilateral's four lines.
    */
    Quadrilateral(Point p1, Point p2, Point p3, Point p4, RGBA c = RGBA(255, 255, 255))
    {
        // printf("quad constructing");
        // c.print();
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

    void getPointsToDraw() override
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
        // properly assign passed points to their corners (we don't need form if we initialize square w/ four points that already respect square properties)
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

class Cuboid : public Shape // start with cube for testing 3d stuff;
{
    // idea is we have 8 vertices and 6 faces
    // we want to apply transformations and then points are ready to draw
    // follow same convention where we have a points to draw vector for the class
    // this vector is added to screen points to draw vector that is rendered at runtime
public:
    Point centroid;
    Point vertices[8];
    std::vector<Line> vertexConnections;
    // need some data structure to hold cuboid faces if I want to implement culling properly

    RGBA color = RGBA(255, 255, 255);

    Cuboid(Point center, float length, float height, float width, RGBA c = RGBA(255, 255, 255))
    {  
        centroid = center;
        color = c;
        // Define the 8 vertices of the cube based on the center and size (FOR NOW USE HARD-CODED Z VALUE FOR TESTING)
        // length corresponds to X distance between faces
        // height corresponds to Y distance bw faces
        // width corresponds to Z distance bw faces

        // Define the 8 vertices
        vertices[0] = Point(center.x - length / 2, center.y - height / 2, center.z.value_or(0) - width / 2, c);
        vertices[1] = Point(center.x + length / 2, center.y - height / 2, center.z.value_or(0) - width / 2, c);
        vertices[2] = Point(center.x + length / 2, center.y + height / 2, center.z.value_or(0) - width / 2, c);
        vertices[3] = Point(center.x - length / 2, center.y + height / 2, center.z.value_or(0) - width / 2, c);
        vertices[4] = Point(center.x - length / 2, center.y - height / 2, center.z.value_or(0) + width / 2, c);
        vertices[5] = Point(center.x + length / 2, center.y - height / 2, center.z.value_or(0) + width / 2, c);
        vertices[6] = Point(center.x + length / 2, center.y + height / 2, center.z.value_or(0) + width / 2, c);
        vertices[7] = Point(center.x - length / 2, center.y + height / 2, center.z.value_or(0) + width / 2, c);

        // Initialize the lines connecting the vertices
        fillLines();
    }

    void fillLines()
    {
        // We don't need faces, just lines connecting each point (these should be reinitialied anytime we update points)

        vertexConnections.clear(); // remove previous lines (if we're calling this method it means vertices have updated; as such, we want to update lines)

        // Connect bottom face vertices
        vertexConnections.push_back(Line(vertices[0], vertices[1], color));
        vertexConnections.push_back(Line(vertices[1], vertices[2], color));
        vertexConnections.push_back(Line(vertices[2], vertices[3], color));
        vertexConnections.push_back(Line(vertices[3], vertices[0], color));

        // Connect top face vertices
        vertexConnections.push_back(Line(vertices[4], vertices[5], color));
        vertexConnections.push_back(Line(vertices[5], vertices[6], color));
        vertexConnections.push_back(Line(vertices[6], vertices[7], color));
        vertexConnections.push_back(Line(vertices[7], vertices[4], color));

        // Connect top face to bottom face
        vertexConnections.push_back(Line(vertices[0], vertices[4], color));
        vertexConnections.push_back(Line(vertices[1], vertices[5], color));
        vertexConnections.push_back(Line(vertices[2], vertices[6], color));
        vertexConnections.push_back(Line(vertices[3], vertices[7], color));
    }

    void getPointsToDraw() override
    {
        for (auto &l : this->vertexConnections)
        {
            for (auto &p : l.pointsToDraw)
            {
                // 
                pointsToDraw.emplace_back(p);
            }
        }
    }

    matrix xRotationMatrix(float xRot)
    {
        matrix rotX = matrix(4, 4);
        rotX.addElement(0, 0, 1);
        rotX.addElement(1, 1, cos(xRot));
        rotX.addElement(1, 2, -sin(xRot));
        rotX.addElement(2, 1, sin(xRot));
        rotX.addElement(2, 2, cos(xRot));
        rotX.addElement(3, 3, 1);
        return rotX;
    }

    matrix yRotationMatrix(float yRot)
    {
        matrix rotY = matrix(4, 4);
        rotY.addElement(0, 0, cos(yRot));
        rotY.addElement(0, 2, sin(yRot));
        rotY.addElement(1, 1, 1);
        rotY.addElement(2, 0, -sin(yRot));
        rotY.addElement(2, 2, cos(yRot));
        rotY.addElement(3, 3, 1);
        return rotY;
    }

    matrix zRotationMatrix(float zRot)
    {
        matrix rotZ = matrix(4, 4);
        rotZ.addElement(0, 0, cos(zRot));
        rotZ.addElement(0, 1, -sin(zRot));
        rotZ.addElement(1, 0, sin(zRot));
        rotZ.addElement(1, 1, cos(zRot));
        rotZ.addElement(2, 2, 1);
        rotZ.addElement(3, 3, 1);
        return rotZ;
    }
    
    void rotate(float xRot, float yRot, float zRot, bool aroundCentroid = true)
    {
        // Rotation matrices
        matrix rotX = xRotationMatrix(xRot);

        matrix rotY = yRotationMatrix(yRot);

        matrix rotZ = zRotationMatrix(zRot);

        // Composite rotation matrix (Rz * Ry * Rx)
        // multiply matrices in this order to ensure we scale z first, then y, then x (then we can just plot x,y but shape will look 3 dimensional)
        matrix rotationMatrix = rotZ.multiply(rotY).multiply(rotX);

        // Rotate each vertex
        // modify value of each point in vertices by mutliplying w/ rotation matrix
        for (int i = 0; i < 8; i++)
        {
            // if rotating around centroid, ensure we translate before and after rotation
            vertices[i].translate(-centroid.x, -centroid.y, -centroid.z.value());
            matrix pointVector = vertices[i].getVector(true); // Get affine point
            matrix rotatedVector = rotationMatrix.multiply(pointVector);
            Point rotatedPoint = vertices[i].getPointFromVector(rotatedVector);
            vertices[i] = rotatedPoint;
            vertices[i].translate(centroid.x, centroid.y, centroid.z.value());
        }
        
        // we need to refill lines after rotating or any transformation operation
        fillLines();
    }
};

// TO-DO:
// implement:
//  - scaling
//  - translation
//  - add Triangle class (should be super straightforward)