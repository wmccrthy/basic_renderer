#include "iostream"
#pragma once

struct RGBA
{
    float r = 255, g = 255, b = 255, a = 255;
    RGBA(float R, float G, float B, float A = 255)
    {
        r = R, g = G, b = B, a = A;
    }

    void print()
    {
        std::cout << "(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(a) + ")\n";
        return;
    }
};