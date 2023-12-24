//
//  raytracer.hpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#ifndef raytracer_hpp
#define raytracer_hpp

#include "beam.hpp"
#include "brightness.hpp"
#include "stage.hpp"
#include <vector>
using namespace std;
using namespace cinder;

struct RayTracer
{
    // common fields
    vector< vector<Beam> > data;
    vector<Object> objects;
    vector<Sphere> spheres;
    unsigned imgWidth, imgHeight;
    BrightnessCalcMethod method;
    // for rsm brightness calculation method
    vector<Transformer> transformers;
    vector<ReflectiveShadowMap> sourceMaps;
    // for tracing brightness calculation method
    vector<LightSource> sources;
    vec3 cameraLocation;
};

void initRayTracer(
    RayTracer& tracer,
    Stage const& stage,
    unsigned imgWidth,
    unsigned imgHeight,
    BrightnessCalcMethod method
);

Surface trace(RayTracer& tracer, unsigned step);

#endif /* raytracer_hpp */
