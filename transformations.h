#include <cstddef>
#include "iostream"
#include "matmul.h"
#pragma once

/*
namespace for transformations
    - at what point do we want to define transformation functions for? (points, lines?)
        - for shapes we want to transform with matrix mult
    - should also support transformations on individual points for convenience sake...
*/
namespace transformations
{
    /*
    translation is straightforward enough; just matrix addition where we add xTranslation, yTranslation and zTranslation
    */
    void translate()
    {
        // create util functions for matrix translation and other operations 
    }

    /*
    scale is straightforward enough; multiply each point by scale matrix (3x3 where each vector is [scaleX, 0, 0][0, scaleY, 0][0 0, scaleZ])
    */
    void scale()
    {

    }

    /*
    implement
    */
    void rotate()
    {

    }

    /*
    implement projection
    */ 
    void project()
    {

    }
}