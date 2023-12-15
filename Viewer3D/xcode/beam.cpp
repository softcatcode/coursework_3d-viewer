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
    if (wereValidRays) {
//        float maxVal = max(max(color.r, color.g), color.b);
//        if (maxVal > 0) {
//            color = (maxVal / min(255.f, maxVal)) * color;
//            result.r = (unsigned char) color.r;
//            result.g = (unsigned char) color.g;
//            result.b = (unsigned char) color.b;
//        }
        result = color * (1 / powerSum);
    }
    return result;
}
