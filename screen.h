#include <SDL2/SDL.h>
#include "point.h"
#include "line.h"
#include <vector>

class Screen
{
    SDL_Event e;
    SDL_Window *window;     // pointer to the window
    SDL_Renderer *renderer; // pointer to renderer
    std::vector<Point> points;
    std::vector<Line> lines;

public:
    void addPixel(float x, float y)
    {
        points.emplace_back(Point(x, y));
    }

    void addLine(float x1, float y1, float x2, float y2)
    {
        Line line = Line(x1, y1, x2, y2);
        for (auto &point : line.pointsToDraw)
        {
            points.emplace_back(point);
        } // add points in line to the vector of points which we draw at render time
    }

    void clearPoints()
    {
        points.clear();
    }

    void displayScreen()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (auto &point : points)
        {
            SDL_RenderDrawPointF(renderer, point.x, point.y); // draw all points in our point vector
        }

        SDL_RenderPresent(renderer); // actually show screen
    }

    void input()
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                SDL_Quit();
                exit(0);
            }
        }
    }

    Screen()
    {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(1080, 720, 0, &window, &renderer);
        SDL_RenderSetScale(renderer, 1.0, 1.0);
    }
};
