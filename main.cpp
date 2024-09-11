#include "screen.h"
#include "point.h"
#include <numeric>
#include "matmul.h"
#include <random>
#include "ctime"

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

    // matrix massiveTest(20, 20);
    // for (int i = 0; i < 20; i ++)
    // {
    //     for (int j = 0; j < 20; j ++)
    //     {
    //         massiveTest[i][j] = rand() % 10;
    //     }
    // }
    // massiveTest.print();

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

    Cuboid specificCuboid = Cuboid(Point(screenWidth / 2, screenHeight / 2, 0), 50, 50, 50);
    Cuboid unlitCuboid = Cuboid(Point(screenWidth / 2, 200, 0), 50, 50, 50, RGBA(255, 255, 255), false, false, true);
    Cuboid wireCuboid = Cuboid(Point(700, screenHeight / 2, 0), 50, 50, 50, RGBA(255, 255, 255), true, false, true);
    Cuboid wireUnculledCuboid = Cuboid(Point(200, screenHeight / 2, 0), 50, 50, 50, RGBA(255, 255, 255), true, true);

    // TESTING TRIANGLE CLASS
    Triangle testTri = Triangle(Point(690, 10), Point(690, 50), Point(650, 10));

    // total render time (for testing)
    float renderTime = 0;
    int numFrames = 0;

    while (true)
    {
        clock_t frameStartTime = clock();
        testScreen.clearPoints();

        // rotate and update points for specific cuboid
        specificCuboid.rotate(0.003, 0.001, 0.004);
        testScreen.updateCuboid(specificCuboid);
        unlitCuboid.rotate(0.003, 0.001, 0.004);
        testScreen.updateCuboid(unlitCuboid);
        wireCuboid.rotate(0.003, 0.001, 0.004);
        testScreen.updateCuboid(wireCuboid);
        wireUnculledCuboid.rotate(0.003, 0.001, 0.004);
        testScreen.updateCuboid(wireUnculledCuboid);

        testScreen.displayScreen(); // Render the updated screen
        testScreen.input(renderTime, numFrames);

        SDL_Delay(0);

        clock_t frameEndTime = clock() - frameStartTime;
        renderTime += (float)frameEndTime / CLOCKS_PER_SEC * 1000;
        numFrames += 1;
        // std::cout << "Frame rendered in: " << (float)frameEndTime/CLOCKS_PER_SEC * 1000 << "ms \n";
    }
    // std::cout << "Average frame render time: " << renderTime / numFrames << "\n";

    return 0;
}