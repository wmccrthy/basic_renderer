#include <SDL2/SDL.h>
#include "color.h"
#include "point.h"
#include "line.h"
#include "quadrilateral.h"
#include <vector>

class Screen
{
    SDL_Event e;
    SDL_Window *window;     // pointer to the window
    SDL_Renderer *renderer; // pointer to renderer
    std::vector<Point> points;
    std::vector<std::unique_ptr<Shape>> shapesInScene; // vector of shapes currently on the Screen; we loop thru these shapes at every frame and render their points

public:
    const int WINDOW_WIDTH = 900;
    const float WINDOW_HEIGHT = 900;
    const float FOV = 90.0f;

    void addPixel(float x, float y)
    {
        points.emplace_back(Point(x, y));
    }

    void addLine(float x1, float y1, float x2, float y2)
    {
        Line line = Line(x1, y1, x2, y2);
        this->addPointsToRender<Line>(line); // add points in line to the vector of points which we draw at render time
    }

    template <class Q>
    void addQuadrilateral(Point p1, Point p2, Point p3, Point p4, RGBA color = RGBA(255, 255, 255))
    {
        Q quad = Q(p1, p2, p3, p4, color);
        this->addPointsToRender<Q>(quad);
    }

    template <class Q>
    void addCuboid(Point center, float length, float height, float width)
    {
        Q cuboid = Q(center, length, height, width);
        cuboid.getPointsToDraw();
        this->addPointsToRender<Q>(cuboid);
    }

    void updateCuboid(Cuboid cuboid)
    {
        cuboid.getPointsToDraw();
        this->addPointsToRender<Cuboid>(cuboid);
    }

    template <class Q>
    void addPointsToRender(Q shape)
    {
        for (auto &p : shape.pointsToDraw)
        {
            points.emplace_back(p);
        }
    }

    // void addShape(std::unique_ptr<Shape> s)
    // {
    //     shapesInScene.emplace_back(std::move(s));
    // }

    // void clearShapes()
    // {
    //     shapesInScene.clear();
    // }

    // void renderShapes()
    // {
    //     for (const auto &shape : shapesInScene)
    //     {
    //         renderShape(*shape);
    //     }
    // }

    // void renderShape(Shape &s)
    // {
    //     s.getPointsToDraw();
    //     for (const auto &p : s.pointsToDraw)
    //     {
    //         renderPoint(p);
    //     }
    // }

    void renderPoint(Point point)
    {
        RGBA color = point.color;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // set draw color to white
        point.projectPerspective(FOV, WINDOW_WIDTH, WINDOW_HEIGHT);
        SDL_RenderDrawPointF(renderer, point.x, point.y);
    }

    void clearPoints()
    {
        points.clear();
    }

    /*
    Render all points in points vector on the screen and display
    */
    void displayScreen()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer); // renders black background

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // set draw color to white

        for (auto &point : points)
        {
            renderPoint(point);
        }

        SDL_RenderPresent(renderer); // actually show screen
    }
    // NOTE: perhaps instead of drawing all points to screen at once, we can have methods that draw single shapes at a time

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
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
        SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
        SDL_RenderSetScale(renderer, 1.0, 1.0);
    }
};
