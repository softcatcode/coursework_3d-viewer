//
//  drawalgodata.hpp
//  Viewer3D
//
//  Created by Daniil on 24.11.2023.
//

#ifndef drawalgodata_hpp
#define drawalgodata_hpp

#include "stage.hpp"
#include "raytracer.hpp"
using namespace cinder;

struct DrawAlgoData
{
    Stage stage;
    RayTracer tracer;
    unsigned imgWidth, imgHeight, step;
    Surface image;
};

#endif /* drawalgodata_hpp */
