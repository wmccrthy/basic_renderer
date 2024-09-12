#include <vector>
#include "iostream"
#include "line.h"
#include "point.h"
#include "math.h"
#include "color.h"
#pragma once

/*
Helper method that returns a line for the Triangle's normal s.t we can render the line for debugging purposes.
*/
Line computeNormalLine(Point centroid, Point normal)
{
    float normalScale = 25.0f; // Adjust the scale factor as needed (adjusts length of the line);
    Point normalEnd = Point(
        centroid.x + normal.x / normalScale,
        centroid.y + normal.y / normalScale,
        centroid.z.value_or(0) + normal.z.value_or(0) / normalScale);
    // Create a line to represent the normal vector
    return Line(centroid, normalEnd, RGBA(255, 0, 0)); // Red color for normal
}

/*
Helper methods that returns rotation matrices given angles of rotation
*/
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
    Point vertices[3];

    Triangle()
    {
    }

    Triangle(Point p1, Point p2, Point p3, RGBA c = RGBA(255, 255, 255))
    {
        color = c;
        this->fillVertices(p1, p2, p3);
        this->fillLines();
        this->getPointsToDraw();
    }

    void fillVertices(Point p1, Point p2, Point p3)
    {
        vertices[0] = p1;
        vertices[1] = p2;
        vertices[2] = p3;
    }

    void fillLines()
    {
        lines[0] = Line(vertices[0], vertices[1]);
        lines[1] = Line(vertices[1], vertices[2]);
        lines[2] = Line(vertices[2], vertices[0]);
    }

    Point computeNormal()
    {
        // compute normal of the triangle using it's vertices (we only need three points to compute normal of plane/face)
        Point vectorA = Line(vertices[0], vertices[1]).getDirectionVector();
        Point vectorB = Line(vertices[0], vertices[2]).getDirectionVector();
        // vectorA.normalize();
        // vectorB.normalize();
        Point normal = vectorA.crossProduct(vectorB);
        // normal.normalize();

        return normal;
    }

    Point computeCentroid()
    {
        return Point(
            (vertices[0].x + vertices[1].x + vertices[2].x) / 3.0f,
            (vertices[0].y + vertices[1].y + vertices[2].y) / 3.0f,
            (vertices[0].z.value_or(0) + vertices[1].z.value_or(0) + vertices[2].z.value_or(0)) / 3.0f);
    }

    void projectTri()
    {
        vertices[0].projectPerspective(60.0f);
        vertices[1].projectPerspective(60.0f);
        vertices[2].projectPerspective(60.0f);
    }

    std::vector<Point> getFillPoints()
    {
        std::vector<Point> filledPoints;

        // Find the bounding box of the triangle (project the points)
        int xMin = std::min({vertices[0].x, vertices[1].x, vertices[2].x});
        int xMax = std::max({vertices[0].x, vertices[1].x, vertices[2].x});
        int yMin = std::min({vertices[0].y, vertices[1].y, vertices[2].y});
        int yMax = std::max({vertices[0].y, vertices[1].y, vertices[2].y});

        // Loop over the bounding box
        for (int y = yMin; y <= yMax; ++y)
        {
            for (int x = xMin; x <= xMax; ++x)
            {
                Point p = Point(x, y, 0.0f);
                // Check if the point is inside the triangle
                if (isPointInTri(p))
                {
                    filledPoints.emplace_back(p);
                }
            }
        }

        return filledPoints;
    }

    bool isPointInTri(Point p)
    {
        // Extract vertices
        Point A = vertices[0];
        Point B = vertices[1];
        Point C = vertices[2];

        // Calculate vectors
        Point v0 = B - A; // Vector from A to B
        Point v1 = C - A; // Vector from A to C
        Point v2 = p - A; // Vector from A to the point

        // Compute dot products
        float dot00 = v0.dotProduct(v0);
        float dot01 = v0.dotProduct(v1);
        float dot02 = v0.dotProduct(v2);
        float dot11 = v1.dotProduct(v1);
        float dot12 = v1.dotProduct(v2);

        // Compute the denominator of the barycentric coordinates
        float denom = dot00 * dot11 - dot01 * dot01;

        // Make sure the triangle is not degenerate (area > 0)
        if (denom == 0)
            return false;

        // Compute barycentric coordinates
        float invDenom = 1.0f / denom;
        float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

        // Check if the point lies within the triangle (u >= 0, v >= 0, and u + v <= 1)
        return (u >= 0) && (v >= 0) && (u + v <= 1);
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

    void print()
    {
        std::cout << "Triangle w/ vertices at:\n";
        for (auto &p : vertices)
        {
            p.print();
        };
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

/*
Can we use the Rectangle class to function as faces for Cuboids?
We could indeed but a face class will be more lightweight than using the rectangle class; also potentially more extensible
*/
struct Face
{
public:
    std::vector<Point> vertices;
    std::vector<Line> lines;
    std::vector<Triangle> polygons;

    Face()
    {
    }

    Face(Point p1, Point p2, Point p3, bool isWireFrame)
    {
        // ORIGINAL IMPLEMENTATION
        vertices.emplace_back(p1);
        vertices.emplace_back(p2);
        vertices.emplace_back(p3);

        // POLYGON/TRIANGLE BASED IMPLEMENTATION
        polygons.emplace_back(Triangle(p1, p2, p3));

        if (isWireFrame)
        {
            fillLines();
        }
    }

    Face(Point p1, Point p2, Point p3, Point p4, bool isWireFrame)
    {
        // ORIGINAL IMPLEMENTATION
        vertices.emplace_back(p1);
        vertices.emplace_back(p2);
        vertices.emplace_back(p3);
        vertices.emplace_back(p4);

        // POLYGON/TRIANGLE BASED IMPLEMENTATION
        polygons.emplace_back(Triangle(p1, p2, p3));
        polygons.emplace_back(Triangle(p1, p3, p4));

        if (isWireFrame)
        {
            fillLines();
        }
    }

    Point computeNormal()
    {
        // ORIGINAL IMPLEMENTATION
        // compute normal of the face using it's vertices (we only need three points to compute normal of plane/face)
        Point vectorA = Line(vertices[0], vertices[1], RGBA(255, 255, 255), true).getDirectionVector();
        Point vectorB = Line(vertices[0], vertices[2], RGBA(255, 255, 255), true).getDirectionVector();
        Point normal = vectorA.crossProduct(vectorB);

        // normal.normalize();

        return normal;

        // TRIANGLE-BASED IMPLEMENTATION
    }

    void projectFace()
    {
        vertices[0].projectPerspective(60.0f);
        vertices[1].projectPerspective(60.0f);
        vertices[2].projectPerspective(60.0f);

        polygons.clear();
        polygons.emplace_back(Triangle(vertices[0], vertices[1], vertices[2]));
        polygons.emplace_back(Triangle(vertices[0], vertices[2], vertices[3]));
    }

    Point computeCentroid()
    {
        float x = 0, y = 0, z = 0;
        for (const auto &vertex : vertices)
        {
            x += vertex.x;
            y += vertex.y;
            z += vertex.z.value_or(0);
        }
        size_t n = vertices.size();
        return Point(x / n, y / n, z / n);
    }

    void fillLines() // each face should have lines that connect it; will be either 3 or 4 (for now)
    {
        // ORIGINAL IMPLEMENTATION
        for (int i = 0; i < vertices.size() - 1; i++)
        {
            lines.emplace_back(Line(vertices[i], vertices[i + 1]));
        }
        // connect final vertex w/ first vertex
        lines.emplace_back(Line(vertices[vertices.size() - 1], vertices[0]));

        // ADDING TRIANGLE LINES
        // for (auto &tri : this->polygons)
        // {
        //     // // tri.print();
        //     // // culling...
        //     // Point cameraPos = Point(0, 0, 0);
        //     // Point triCentroid = tri.computeCentroid();
        //     // // tri.projectTri();
        //     // Point triNormal = tri.computeNormal();
        //     // // RENDER THE TRIANGLE'S NORMAL FOR DEBUGGING PURPOSES
        //     // // lines.emplace_back(computeNormalLine(triCentroid, triNormal));

        //     // // Normalize centroid and translate according to cameraPos
        //     // triCentroid.normalize();
        //     // triCentroid.translate(-cameraPos.x, -cameraPos.y, -cameraPos.z.value_or(0));

        //     // float viewVector = triNormal.dotProduct(triCentroid);
        //     // std::cout << viewVector << "\n";
        //     // float epsilon = 1e-4; // Small threshold to account for precision errors

        //     if (true)
        //     {
        //         for (int i = 0; i < 3; i++)
        //         {
        //             lines.emplace_back(tri.lines[i]);
        //         }
        //     }
        // }
    }

    /*
    Gets all points that 'fill' the face (make it solid) by projecting the face's vertices and scanning over the face's bounding box
    */
    std::vector<Point> getFillPoints()
    {
        std::vector<Point> filledPoints;
        // first project vertices s.t they are in 2d screen space
        Point v0 = vertices[0].getProjected(60.0f);
        Point v1 = vertices[1].getProjected(60.0f);
        Point v2 = vertices[2].getProjected(60.0f);
        Point v3 = vertices[3].getProjected(60.0f);

        // Get the bounding box for the face
        int xMin = std::min({v0.x, v1.x, v2.x, v3.x});
        int xMax = std::max({v0.x, v1.x, v2.x, v3.x});
        int yMin = std::min({v0.y, v1.y, v2.y, v3.y});
        int yMax = std::max({v0.y, v1.y, v2.y, v3.y});

        // Loop over the bounding box and check each point
        for (int y = yMin; y <= yMax; ++y)
        {
            for (int x = xMin; x <= xMax; ++x)
            {
                Point p(x, y);

                // Check if the point is inside the quadrilateral
                if (isPointInQuad(p, v0, v1, v2, v3))
                {
                    filledPoints.emplace_back(p);
                }
            }
        }

        return filledPoints;
    }

    bool isPointInTri(const Point p, Point v0, Point v1, Point v2)
    {
        // Barycentric coordinate method

        float denom = (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y);
        float a = ((v1.y - v2.y) * (p.x - v2.x) + (v2.x - v1.x) * (p.y - v2.y)) / denom;
        float b = ((v2.y - v0.y) * (p.x - v2.x) + (v0.x - v2.x) * (p.y - v2.y)) / denom;
        float c = 1.0f - a - b;

        return a >= 0 && b >= 0 && c >= 0;
    }

    bool isPointInQuad(const Point &p, Point v0, Point v1, Point v2, Point v3)
    {
        // Split quadrilateral into two triangles (v0, v1, v2) and (v0, v2, v3)
        bool inFirstTriangle = isPointInTri(p, v0, v1, v2);
        bool inSecondTriangle = isPointInTri(p, v0, v2, v3);

        return inFirstTriangle || inSecondTriangle;
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
    Face faces[6];
    bool isWireFrame;
    bool dontCull;
    bool dontLight;

    RGBA color = RGBA(255, 255, 255);

    Cuboid(Point center, float length, float height, float width, RGBA c = RGBA(255, 255, 255), bool isWireFrame = false, bool dontCull = false, bool dontLight = false)
    {
        centroid = center;
        color = c;
        this->isWireFrame = isWireFrame;
        this->dontCull = dontCull;
        this->dontLight = dontLight;

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

        // Initialize the faces connecting the vertices
        fillFaces();
    }

    void fillFaces()
    {
        // fill faces based on vertices
        // Bottom face (vertices[0], vertices[1], vertices[2], vertices[3])
        faces[0] = Face(vertices[0], vertices[3], vertices[2], vertices[1], isWireFrame);

        // Top face (vertices[4], vertices[5], vertices[6], vertices[7])
        faces[1] = Face(vertices[4], vertices[5], vertices[6], vertices[7], isWireFrame);

        // Front face (vertices[0], vertices[1], vertices[5], vertices[4])
        faces[2] = Face(vertices[0], vertices[1], vertices[5], vertices[4], isWireFrame);

        // Back face (vertices[2], vertices[3], vertices[7], vertices[6])
        faces[3] = Face(vertices[2], vertices[3], vertices[7], vertices[6], isWireFrame);

        // Left face (vertices[3], vertices[0], vertices[4], vertices[7])
        faces[4] = Face(vertices[3], vertices[0], vertices[4], vertices[7], isWireFrame);

        // Right face (vertices[1], vertices[2], vertices[6], vertices[5])
        faces[5] = Face(vertices[1], vertices[2], vertices[6], vertices[5], isWireFrame);
    }

    void getPointsToDraw() override
    {
        // for now use basic lighting approach (light source from camera)
        Point lightingVector = Point(0, 0, 1.0f);

        // testing faces
        for (auto &f : this->faces)
        {
            // previously, we were culling based on the normal of the un-projected face; this led to issues so we now
            // try culling against the projected face's normal (project each vertex on f)
            Face faceCopy = Face(f.vertices[0], f.vertices[1], f.vertices[2], f.vertices[3], isWireFrame);
            faceCopy.projectFace();
            Point faceNormal = faceCopy.computeNormal();
            // Culling condition
            if (faceNormal.z > 0 | dontCull)
            {
                // compute face color based on lighting vector (for now just vector pointing in z direction)
                // i assume we want to dot the faceNormal with the lighting vector to compute the 'brightness' value
                float brightness = lightingVector.dotProduct(faceNormal.stdNormalize());
                RGBA lightingColor = (dontCull | dontLight) ? color : RGBA(color.r * brightness, color.g * brightness, color.b * brightness);
                // std::cout << "lighting color:\n";
                // lightingColor.print();

                if (isWireFrame)
                {
                    for (auto &l : f.lines)
                    {
                        for (auto &p : l.pointsToDraw)
                        {
                            p.color = lightingColor;
                            pointsToDraw.emplace_back(p);
                        }
                    }
                }
                else
                {
                    // IN PROGRESS: ADD POINTS THAT MAKE SHAPE SOLID
                    for (auto &solidP : f.getFillPoints())
                    {
                        solidP.color = lightingColor;
                        pointsToDraw.emplace_back(solidP);
                    }
                }

                // RENDER THE NORMAL FOR DEBUGGING (seems to b working oddly now)
                // Line normalLine = computeNormalLine(f.computeCentroid(), f.computeNormal());
                // for (auto &p : normalLine.pointsToDraw)
                // {
                //     pointsToDraw.emplace_back(p);
                // }
            }
        }
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
            if (aroundCentroid)
            {
                vertices[i].translate(-centroid.x, -centroid.y, -centroid.z.value());
            }

            matrix pointVector = vertices[i].getVector(true); // Get affine point
            matrix rotatedVector = rotationMatrix.multiply(pointVector);
            Point rotatedPoint = vertices[i].getPointFromVector(rotatedVector);
            vertices[i] = rotatedPoint;

            if (aroundCentroid)
            {
                vertices[i].translate(centroid.x, centroid.y, centroid.z.value());
            }
        }

        // we need to refill lines after rotating or any transformation operation
        fillFaces();
    }
};

// TO-DO:
// implement:
//  - scaling
//  - translation
//  - add Triangle class (should be super straightforward)