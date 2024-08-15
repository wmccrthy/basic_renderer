#include "screen.h"

int main() {
    Screen testScreen;

    for (int i = 0; i < 250; i ++) {
        testScreen.addPixel(rand() % 1080, rand() % 720);
    }
    while (true) {
        testScreen.displayScreen();
        testScreen.input();
    }
    return 0;
}