#include "screen.h"

int main()
{
    Screen testScreen;

    // for (int i = 0; i < 250; i++)
    // {
    //     testScreen.addPixel(rand() % 1080, rand() % 720);
    // }

    // for (int i = 0; i < 5; i++)
    // {
    //     testScreen.addLine(rand() % 1080, rand() % 720, rand() % 1080, rand() % 720);
    // }

    testScreen.addLine(100, 100, 100, 200);
    testScreen.addLine(100, 200, 200, 200);
    testScreen.addLine(200, 200, 200, 100);
    testScreen.addLine(200, 100, 100, 100);

    // can add pixel to the screen to be drawn with

    while (true)
    {
        testScreen.displayScreen();
        testScreen.input();
    }
    return 0;
}