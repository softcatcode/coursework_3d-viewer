//
//  raymanager.cpp
//  Viewer3D
//
//  Created by Daniil on 25.11.2023.
//

#include "beam.hpp"
#include "parameters.hpp"
#include "brightness.hpp"

void initBeam(Beam& beam, vec3 const& point, vec3 const& direction)
{
    beam = { {Ray(point, direction), backgroundColor, 1.f, 0} };
}

Color getColor(Beam const& beam)
{
    Color color(0.f, 0.f, 0.f);
    bool wereValidRays = false;
    float powerSum = EPS;
    for (auto const& seg: beam) {
        if (seg.collisionCount > 0) {
            color += seg.color * seg.power;
            powerSum += seg.power;
            wereValidRays = true;
        }
    }
    Color result = backgroundColor;
    if (wereValidRays)
        result = color / powerSum;
    return result;
}
