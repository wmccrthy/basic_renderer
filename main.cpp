#include "screen.h"
#include "point.h"
#include <numeric>

int main()
{
    Screen testScreen;
    int screenWidth = testScreen.WINDOW_WIDTH, screenHeight = testScreen.WINDOW_HEIGHT;

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
        Point p1 = Point(rand() % screenWidth, rand() % screenHeight);
        Point p2 = Point(rand() % screenWidth, rand() % screenHeight);
        Point p3 = Point(rand() % screenWidth, rand() % screenHeight);
        Point p4 = Point(rand() % screenWidth, rand() % screenHeight);

        // hardcode four points for testing
        // Point p1 = Point(480, 390);
        // Point p2 = Point(595, 430);
        // Point p3 = Point(650, 300);
        // Point p4 = Point(500, 330);

        // call addQuadrilateral passing four randomized points
        testScreen.addQuadrilateral<Quadrilateral>(p1, p2, p3, p4);
        testScreen.addQuadrilateral<Rectangle>(p1, p2, p3, p4);
        testScreen.addQuadrilateral<Square>(p1, p2, p3, p4);
    }

    while (true)
    {
        testScreen.displayScreen();
        testScreen.input();
        SDL_Delay(50);
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
            - drawSquare(topLeft, anchorPoint)
            - and so on...

    at this point, the four points we have are the four points that make up the Square
    need to understand 3d transformation stuff:
     - transform?
     - rotate?
     - projection?

    How are we representing/abstracting the z axis?

*/