//
//  objproperties.hpp
//  Viewer3D
//
//  Created by Daniil on 25.11.2023.
//

#ifndef objproperties_hpp
#define objproperties_hpp

using namespace cinder;

#include "parameters.hpp"

struct ObjectProperties
{
    Color color;
    float transmission, reflection, optDensity;
};

#endif /* objproperties_hpp */
