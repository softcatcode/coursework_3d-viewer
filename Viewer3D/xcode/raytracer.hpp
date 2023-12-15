//
//  raytracer.hpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#ifndef raytracer_hpp
#define raytracer_hpp

#include "beam.hpp"
#include "stage.hpp"
#include <vector>
using namespace std;
using namespace cinder;

struct RayTracer
{
    vector< vector<Beam> > data;
    vector<Sphere> spheres;
    vector<Transformer> transformers;
    vector<ReflectiveShadowMap> sourceMaps;
    vector<Object> objects;
    vector<LightSource> sources;
    unsigned imgWidth, imgHeight;
};

void initRayTracer(
    RayTracer& tracer,
    Stage const& stage,
    unsigned imgWidth, unsigned imgHeight
);

Surface trace(RayTracer& tracer, unsigned step);

#endif /* raytracer_hpp */
