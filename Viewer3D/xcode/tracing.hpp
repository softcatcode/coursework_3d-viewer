//
//  tracing.hpp
//  Viewer3D
//
//  Created by Daniil on 14.12.2023.
//

#ifndef tracing_hpp
#define tracing_hpp

#include <vector>
#include "shadowmap.hpp"
#include "mathutils.hpp"
#include "beam.hpp"
using namespace std;

Color traceRay(
    Beam& beam,
    vector<Object> const& objects,
    vector<Sphere> const& spheres,
    vector<ReflectiveShadowMap> const& shadowMaps,
    vector<Transformer> const& transformers
);

Color traceRay(
    Beam& beam,
    vector<Object> const& objects,
    vector<Sphere> const& spheres,
    vector<LightSource> const& sources
);

#endif /* tracing_hpp */
