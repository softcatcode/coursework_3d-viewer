//
//  brightness.hpp
//  Viewer3D
//
//  Created by Daniil on 14.12.2023.
//

#ifndef brightness_hpp
#define brightness_hpp

#include <vector>
#include "shadowmap.hpp"
#include "mathutils.hpp"
using namespace std;

float calculateBrightness(
    Collision const& collision,
    vector<ReflectiveShadowMap> const& shadowMaps,
    vector<Transformer> transformers
);

float traceBrightness(
    Collision const& collision,
    vector<LightSource> const& sources,
    vector<Object> const& objects,
    vector<Sphere> const& spheres
);

#endif /* brightness_hpp */
