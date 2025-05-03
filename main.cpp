#include "screen.h"
#include "point.h"
#include <numeric>
#include "matmul.h"
#include <random>
#include "ctime"

int main()
{
    Screen testScreen;
    int screenWidth = testScreen.WINDOW_WIDTH;
    int screenHeight = testScreen.WINDOW_HEIGHT;

    // Create a square in the center of the screen
    float squareSize = 100.0f;
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;
    
    // Create the square's vertices
    Point p1 = Point(centerX - squareSize/2, centerY - squareSize/2);
    Point p2 = Point(centerX + squareSize/2, centerY - squareSize/2);
    Point p3 = Point(centerX + squareSize/2, centerY + squareSize/2);
    Point p4 = Point(centerX - squareSize/2, centerY + squareSize/2);

    // Create the square with red color
    Square centerSquare = Square(p1, p2, p3, p4, RGBA(255, 0, 0));

    // Total render time tracking
    float renderTime = 0;
    int numFrames = 0;
    float rotationAngle = 0.0f;

    while (true)
    {
        clock_t frameStartTime = clock();
        testScreen.clearPoints();

        // Rotate the square
        rotationAngle += 0.01f; // Adjust rotation speed as needed
        
        // Update square vertices with rotation
        matrix rotZ = zRotationMatrix(rotationAngle);
        
        // Convert points to vectors, apply rotation, and convert back
        matrix p1Vec = p1.getVector(true);
        matrix p2Vec = p2.getVector(true);
        matrix p3Vec = p3.getVector(true);
        matrix p4Vec = p4.getVector(true);
        
        p1 = getPointFromVector(rotZ.multiply(p1Vec));
        p2 = getPointFromVector(rotZ.multiply(p2Vec));
        p3 = getPointFromVector(rotZ.multiply(p3Vec));
        p4 = getPointFromVector(rotZ.multiply(p4Vec));
        
        // Update the square with new vertices
        centerSquare = Square(p1, p2, p3, p4, RGBA(255, 0, 0));
        
        // Add the square to the screen
        testScreen.addQuadrilateral<Square>(p1, p2, p3, p4, RGBA(255, 0, 0));

        // Display the scene and handle input
        testScreen.displayScreen();
        testScreen.input(renderTime, numFrames);

        // Calculate frame time
        clock_t frameEndTime = clock() - frameStartTime;
        renderTime += (float)frameEndTime / CLOCKS_PER_SEC * 1000;
        numFrames += 1;

        // Optional: Print FPS every 60 frames
        if (numFrames % 60 == 0) {
            std::cout << "Average FPS: " << 1000.0f / (renderTime / numFrames) << "\n";
        }
    }

    return 0;
}