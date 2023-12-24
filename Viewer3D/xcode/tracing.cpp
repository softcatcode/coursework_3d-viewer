//
//  tracing.cpp
//  Viewer3D
//
//  Created by Daniil on 14.12.2023.
//

#include "tracing.hpp"
#include "raytracer.hpp"
#include "beam.hpp"
#include "brightness.hpp"

Color traceBeam(
    Beam& beam,
    vector<Object> const& objects,
    vector<Sphere> const& spheres,
    BrightnessCalcArgs const& brCalcArgs
) {
    size_t n = beam.size();
    for (size_t i = 0; i < n; i++) {
        bool passedToInfinity = false;
        while (accuracyNotReached(beam[i]) && !passedToInfinity) {
            auto searchResPtr = findNextCollision(beam[i], objects, spheres);
            if (searchResPtr) {
                Collision collision = searchResPtr->first;
                unsigned objIndex = searchResPtr->second;
                brCalcArgs.methodRSMArgs.putArg1(collision);
                collision.color = calculateBrightness(brCalcArgs);
                auto segments = split(beam[i], collision);
                beam[i] = segments.first;
                beam.push_back(segments.second);
            } else
                passedToInfinity = true;
        }
    }
    return getColor(beam);
}
