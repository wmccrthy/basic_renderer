#include <SDL2/SDL.h>
#include "point.h"
#include <vector>

class Screen {
    SDL_Event e;
    SDL_Window* window; //pointer to the window
    SDL_Renderer* renderer; //pointer to renderer
    std::vector<Point> points;

    public:
            void addPixel(float x, float y) {
                points.emplace_back(Point(x, y));
            }

            void clearPoints() {
                points.clear();
            }

            void displayScreen() {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                for (auto& point : points) {
                    SDL_RenderDrawPointF(renderer, point.x, point.y); // draw all points in our point vector
                } 

                SDL_RenderPresent(renderer); // actually show screen
            }

            void input() {
                while(SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        SDL_Quit();
                        exit(0);
                    }
                }
            }

        Screen() {        
            SDL_Init(SDL_INIT_VIDEO);
            SDL_CreateWindowAndRenderer(1080, 720, 0, &window, &renderer);
            SDL_RenderSetScale(renderer, 1.0, 1.0);
        };
};

