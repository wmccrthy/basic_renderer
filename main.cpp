#include "screen.h"
#include "point.h"
#include <numeric>
#include "matmul.h"
#include <random>

int main()
{
    Screen testScreen;
    int screenWidth = testScreen.WINDOW_WIDTH, screenHeight = testScreen.WINDOW_HEIGHT;

    // testing point class
    Point testPoint3D = Point(3, 5, 10), testPoint2D = Point(1, 6);
    testPoint3D.print();
    testPoint2D.print();
    matrix testPointVector3D = testPoint3D.getVector(), testPointVector2D = testPoint2D.getVector();
    testPointVector2D.print();
    testPointVector3D.print();
    matrix transposeTest = testPointVector3D.transpose();
    transposeTest.print();
    testPointVector3D.multiply(transposeTest).print();
    transposeTest.multiply(testPointVector3D).print();

    // test matrix implementation
    matrix test1(2, 2);
    test1[0][0] = 1;
    test1[0][1] = 3;
    test1[1][0] = 2;
    test1[1][1] = 2;
    matrix test2(2, 2);
    test2[0][0] = 1;
    test2[0][1] = 3;
    test2[1][0] = 2;
    test2[1][1] = 2;
    test1.print();
    matrix test3 = test1.multiply(test2);
    test3.print();
    matrix test4 = test3.multiply(test1);
    test4.print();
    matrix test5 = test4.add(test1);
    test5.print();
    matrix test6 = test5.multiply(testPointVector2D);
    test6.print();
    // Draws random pixels
    // for (int i = 0; i < 250; i++)
    // {
    //     testScreen.addPixel(rand() % 1080, rand() % 720);
    // }

    // Draws random lines
    // for (int i = 0; i < 5; i++)
    // {
    //     testScreen.addLine(rand() % 1080, rand() % 720, rand() % 1080, rand() % 720);
    // }

    // Draws Rectangle using Lines
    // testScreen.addLine(100, 100, 100, 200);
    // testScreen.addLine(100, 200, 200, 200);
    // testScreen.addLine(200, 200, 200, 100);
    // testScreen.addLine(200, 100, 100, 100);

    // Draws random quadrilaterals
    for (int i = 0; i < 3; i++)
    {
        // randomize four points
        // Point p1 = Point(rand() % screenWidth, rand() % screenHeight);
        // Point p2 = Point(rand() % screenWidth, rand() % screenHeight);
        // Point p3 = Point(rand() % screenWidth, rand() % screenHeight);
        // Point p4 = Point(rand() % screenWidth, rand() % screenHeight);

        // hardcode four points for testing
        Point p1 = Point(390, 390);
        Point p2 = Point(430, 430);
        Point p3 = Point(650, 300);
        Point p4 = Point(500, 330);

        // call addQuadrilateral passing four randomized points
        testScreen.addQuadrilateral<Quadrilateral>(p1, p2, p3, p4, RGBA(255, 0, 0));
        testScreen.addQuadrilateral<Rectangle>(p1, p2, p3, p4, RGBA(0, 255, 0));
        testScreen.addQuadrilateral<Square>(p1, p2, p3, p4, RGBA(0, 0, 255));

    }

    // Test adding random cuboids
    std::vector<Cuboid> cuboids;
    for (int i = 0; i < 10; i++)
    {
        cuboids.emplace_back(Cuboid(
            Point(rand() % screenWidth, rand() % screenHeight, rand() % screenWidth),
            rand() % 100, rand() % 100, rand() % 100,
            RGBA(rand() % 255, rand() % 255, rand() % 255)));
    }

    std::random_device rd;
    std::default_random_engine generator(rd()); // rd() provides a random seed
    std::uniform_real_distribution<float> distribution(0.01, 0.1);

    Cuboid specificCuboid = Cuboid(Point(screenWidth / 2, screenHeight / 2, 0), 50, 50, 300);
    Cuboid originCuboid = Cuboid(Point(200, 200, 900), 50, 50, 100);
    Cuboid originCuboidClose = Cuboid(Point(200, 200, 100), 50, 50, 100);

    // TESTING TRIANGLE CLASS
    Triangle testTri = Triangle(Point(690, 10), Point(690, 50), Point(650, 10));

    while (true)
    {
        testScreen.clearPoints();

        // Rotate and update points for each random cuboid
        // for (auto& c : cuboids)
        // {
        //     c.rotate(distribution(generator), distribution(generator), distribution(generator));
        //     testScreen.updateCuboid(c);
        // }

        // rotate and update points for speciic cuboid
        specificCuboid.rotate(0.02, 0.01, 0.01);
        testScreen.updateCuboid(specificCuboid);
        originCuboid.rotate(0.04, 0.03, -0.05);
        testScreen.updateCuboid(originCuboid);
        originCuboidClose.rotate(0.05, 0.01, -0.02);
        testScreen.updateCuboid(originCuboidClose);
        testScreen.addPointsToRender<Triangle>(testTri);

        testScreen.displayScreen(); // Render the updated screen
        testScreen.input(); 

        SDL_Delay(35);
    }
    return 0;
}

/*
AGENDA:
    3D TIME!
    To-Do:
        - make Drawer class
            - generally idea for this is given a to draw certain shapes
            - drawLine(p1, p2)
            - drawQuadrilateral(p1, p2, p3, p4)
            - drawRectangle(topLeft, width, height)
            - drawSquare(topLeft, sideLength)
            - drawTriangle(p1, p2, p3)
            - and so on...

    at this point, the four points we have are the four points that make up the Square
    need to understand 3d transformation stuff:
     - transform?
     - rotate?
     - projection?

    How are we representing/abstracting the z axis?
    see youtube video (cracked) we normalize all coords to (-1, 1) range

    IMPLEMENT MATRIX MULTIPLICATION

    IMPLEMENT PERSPECTIVE RPOJECTION
*/