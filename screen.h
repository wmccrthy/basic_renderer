#include <SDL2/SDL.h>
#include "color.h"
#include "point.h"
#include "line.h"
#include "quadrilateral.h"
#include <vector>
#include <cmath>

class Camera
{
public:
        // TRANSFORM POINT RELATIVE TO THE CAMERA POSITION & DIRECTION HERE 
        
        Point cameraPos = Point(0, 0, 0);
        Point worldSpaceOrigin = Point(0, 0, 0);
        Point cameraCenterView = worldSpaceOrigin;
        // for now, i want camera to always be centered (world moves around camera)
        // to find updated (worldSpaceCameraCenter), I want to rotate a vector that points in the z direction
        Point cameraDirection = Point(0, 0, -1.0f); // Changed to negative Z for proper forward direction
        Point cameraUp = Point(0, 1.0f, 0); // y axis for camera 
        Point cameraRight = Point(1.0f, 0, 0); // x axis for camera 
        Line forwardNormal;
        Line rightNormal;
        Line upNormal;

        float pitch = 0;
        
        float yaw = 0;

        // how do I get vectors that Point in certain directions relative to camera orientation...?
        
        // REPRESENT CAMERA AS SET OF VECTORS (INITIALLY {0, 0, 1.0F} INDICATING WE ARE FACING FORWARD)

        // DIRECTION OF MOVEMENT IS DERIVED FROM CAMERA VECTOR (UP ARROW -> MOVE IN DIR OF VECTOR, LEFT ARROW -> MOVE IN DIR OF 270DEG FROM VECTOR DIR, RIGHT ARROW -> MOVE IN DIR OF 90DEG FROM VECTOR DIR)
        // THESE SHOULD JUST BE TRANSLATIONS

        // CHANGE CAMERA VECTOR (SIMULATES LOOKING AROUND) BY APPLYING ROTATION

        // NEED TO UPDATE CULLING ACCORDING TO CAMERA POS! (prob need to pass to cuboid...?)
        
        Camera()
        {
            updateVectors();
        }

        void printVectors()
        {
            std::cout << "Camera Vectors\n";
            cameraDirection.print();
            cameraUp.print();
            cameraRight.print();
        }

        void updateVectors()
        {
            // First normalize the camera direction
            cameraDirection = cameraDirection.stdNormalize();
            
            // Compute right vector (perpendicular to up and direction)
            cameraRight = Point(0, 1.0f, 0).crossProduct(cameraDirection);
            cameraRight = cameraRight.stdNormalize();
            
            // Compute up vector (perpendicular to direction and right)
            cameraUp = cameraDirection.crossProduct(cameraRight);
            cameraUp = cameraUp.stdNormalize();

            // Update debug lines with proper scaling
            float debugScale = 50.0f; // Adjust this value to make the debug lines more visible
            forwardNormal = Line(Point(450.0f, 450.0f, 0), 
                               Point(450.0f, 450.0f, 0) + (cameraDirection * debugScale), 
                               RGBA(255, 0, 0));
            rightNormal = Line(Point(450.0f, 450.0f, 0), 
                              Point(450.0f, 450.0f, 0) + (cameraRight * debugScale), 
                              RGBA(0, 255, 0));
            upNormal = Line(Point(450.0f, 450.0f, 0), 
                           Point(450.0f, 450.0f, 0) + (cameraUp * debugScale), 
                           RGBA(0, 0, 255));
        }

        void rotateCamera(float yawChange, float pitchChange)
        {
            // rotate cameraDirection in Y by yaw
            yaw += yawChange;
            pitch += pitchChange;
            matrix rotY = yRotationMatrix(yaw);
            matrix rotX = xRotationMatrix(pitch);
            matrix finalRot = rotY.multiply(rotX);
            cameraDirection = getPointFromVector(finalRot.multiply(Point(0, 0, 1.0f).getVector(true)));

            updateVectors();
        }

        matrix cameraViewMatrix()
        {
            updateVectors();
            
            // Create the rotation part of the view matrix
            matrix cameraRotation = matrix(4, 4);
            
            // First row: Right vector
            cameraRotation[0][0] = cameraRight.x;
            cameraRotation[0][1] = cameraRight.y;
            cameraRotation[0][2] = cameraRight.z.value();
            cameraRotation[0][3] = 0;

            // Second row: Up vector
            cameraRotation[1][0] = cameraUp.x;
            cameraRotation[1][1] = cameraUp.y;
            cameraRotation[1][2] = cameraUp.z.value();
            cameraRotation[1][3] = 0;

            // Third row: Negative forward vector (since we look down -Z)
            cameraRotation[2][0] = -cameraDirection.x;
            cameraRotation[2][1] = -cameraDirection.y;
            cameraRotation[2][2] = -cameraDirection.z.value();
            cameraRotation[2][3] = 0;

            // Fourth row: Position
            cameraRotation[3][0] = 0;
            cameraRotation[3][1] = 0;
            cameraRotation[3][2] = 0;
            cameraRotation[3][3] = 1;

            return cameraRotation;
        }
};

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
    const float FOV = 60.0f;
    Camera camera = Camera();
    
    bool moveUp = false;
    bool moveDown = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveForward = false;
    bool moveBackward = false;
    bool rotateRight = false;
    bool rotateLeft = false;
    bool rotateUp = false;
    bool rotateDown = false;

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
        cuboid.getPointsToDraw(Point(0, 0, 0), Point(0, 0, 1.0f));
        this->addPointsToRender<Q>(cuboid);
    }

    void updateCuboid(Cuboid cuboid)
    {
        // update relative to camera
        this->updateRelativeToCamera(cuboid, 8);
        cuboid.getPointsToDraw(camera.cameraPos, camera.cameraDirection);
        this->addPointsToRender<Cuboid>(cuboid);
    }

    template <class Q>
    void updateRelativeToCamera(Q &shape, int numVertices)
    {
        matrix cameraTransform = camera.cameraViewMatrix();

        for (int i = 0; i < numVertices; i++)
        {
            // Convert vertex to homogeneous coordinates
            matrix vertexVector = shape.vertices[i].getVector(true);
            
            // Apply camera transformation
            matrix transformedVertex = cameraTransform.multiply(vertexVector);
            
            // Convert back to 3D point and apply perspective division
            Point transformedPoint = getPointFromVector(transformedVertex);
            if (transformedVertex[0][3] != 0) {
                transformedPoint = transformedPoint / transformedVertex[0][3];
            }
            
            // Update the vertex
            shape.vertices[i] = transformedPoint;
        }

        shape.fillFaces();
    }

    template <class Q>
    void addPointsToRender(Q shape)
    {
        for (auto &p : shape.pointsToDraw)
        {
            points.emplace_back(p);
        }
    }


    void renderPoint(Point point)
    {
        RGBA color = point.color;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // set draw color to white


        // recall: this check is obsolete given new Cuboid implementation where we project points in getFillPoints()
        if (point.z.has_value())
        {
            point.projectPerspective(FOV);
        }
        // point.print();
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

        // render camera direction nnormal for debugging
        // camera.forwardNormal.print();
        for (auto &p : camera.forwardNormal.pointsToDraw)
        {
            renderPoint(p);
        }
        for (auto &p : camera.upNormal.pointsToDraw)
        {
            renderPoint(p);
        }
        for (auto &p : camera.rightNormal.pointsToDraw)
        {
            renderPoint(p);
        }

        SDL_RenderPresent(renderer); // actually show screen
    }
    // NOTE: perhaps instead of drawing all points to screen at once, we can have methods that draw single shapes at a time

    void input(float renderTime, int numFrames)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                SDL_Quit();
                std::cout << "Average frame render time: " << renderTime / numFrames << "ms\n";
                exit(0);
            }
            if (e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym){
                    case SDLK_a:
                        moveLeft = true;
                        resetBools(moveUp, moveRight, moveDown, moveForward, moveBackward, rotateLeft, rotateRight, rotateUp, rotateDown);
                        break;
                    case SDLK_d:
                        moveRight = true;
                        resetBools(moveUp, moveDown, moveLeft, moveForward, moveBackward, rotateLeft, rotateRight, rotateUp, rotateDown);
                        break;
                    case SDLK_w:
                        moveForward = true;
                        resetBools(moveDown, moveRight, moveLeft, moveUp, moveBackward, rotateLeft, rotateRight, rotateUp, rotateDown);
                        break;
                    case SDLK_s:
                        moveBackward = true;
                        resetBools(moveUp, moveRight, moveLeft, moveForward, moveDown, rotateLeft, rotateRight, rotateUp, rotateDown);
                        break;
                    case SDLK_UP:
                        rotateUp = true;
                        resetBools(moveUp, moveRight, moveLeft, moveDown, moveBackward, rotateLeft, rotateRight, rotateDown, moveForward);
                        break;
                    case SDLK_DOWN:
                        rotateDown = true;
                        resetBools(moveUp, moveRight, moveLeft, moveDown, moveForward, rotateLeft, rotateRight, rotateUp, moveBackward);
                        break;
                    case SDLK_RIGHT:
                        rotateRight = true;
                        resetBools(moveUp, moveRight, moveLeft, moveDown, moveForward, rotateLeft, moveBackward, rotateUp, rotateDown);
                        break;
                    case SDLK_LEFT:
                        rotateLeft = true;
                        resetBools(moveUp, moveRight, moveLeft, moveDown, moveForward, moveBackward, rotateRight, rotateUp, rotateDown);
                    default:
                        break;
                }
            }
        }
        updateCamera(moveUp, moveDown, moveLeft, moveRight, moveForward, moveBackward, rotateRight, rotateLeft, rotateUp, rotateDown);
    }

    void resetBools(bool &b1, bool &b2, bool &b3, bool &b4, bool &b5, bool &b6, bool &b7, bool &b8, bool &b9)
    {
        b1 = false;
        b2 = false;
        b3 = false;
        b4 = false;
        b5 = false;
        b6 = false,
        b7 = false;
        b8 = false;
        b9 = false;
    }

    void updateCamera(bool moveUp, bool moveDown, bool moveLeft, bool moveRight, bool moveForward, bool moveBackward, bool rotateRight, bool rotateLeft, bool rotateUp, bool rotateDown)
    {
        if (moveUp)
        {
            camera.rotateCamera(0, 0.005);
        }
        if (moveDown)
        {
            camera.rotateCamera(0, -0.005);
        }
        if (moveLeft)
        {
            camera.rotateCamera(-0.005, 0);
        }
        if (moveRight)
        {
            camera.rotateCamera(0.005, 0);
        }
        if (moveForward)
        {
            camera.rotateCamera(0, 0.005);
        }
        if (moveBackward)
        {
            camera.rotateCamera(0, -0.005);
        }
        if (rotateRight)
        {
            camera.rotateCamera(0.005, 0);
        }
        if (rotateLeft)
        {
            camera.rotateCamera(-0.005, 0);
        }
        if (rotateUp)
        {
            camera.rotateCamera(0, 0.005);
        }
        if (rotateDown)
        {
            camera.rotateCamera(0, -0.005);
        }
    }

    Screen()
    {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
        SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, SDL_RENDERER_PRESENTVSYNC, &window, &renderer);
        SDL_RenderSetScale(renderer, 1.0, 1.0);
    }
};
