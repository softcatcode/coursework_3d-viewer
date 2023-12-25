//
//  beam.cpp
//  Viewer3D
//
//  Created by Daniil on 25.11.2023.
//

#include "beam.hpp"
#include "parameters.hpp"
#include "brightness.hpp"

void initBeam(Beam& beam, vec3 const& point, vec3 const& direction)
{
    beam = { {Ray(point, direction), backgroundColor, 0} };
}

Color getColor(Beam const& beam)
{
    Color color(0.f, 0.f, 0.f);
    unsigned count = 0U;
    for (auto const& seg: beam) {
        if (seg.collisionCount > 0) {
            color += seg.color;
            ++count;
        }
    }
    Color result;
    if (count > 0)
        result = color / count;
    else
        result = backgroundColor;
    return result;
}
