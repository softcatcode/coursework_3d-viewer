//
//  tracing.cpp
//  Viewer3D
//
//  Created by Daniil on 14.12.2023.
//

#include "tracing.hpp"
#include "beam.hpp"
#include "brightness.hpp"

Color traceRay(
    Beam& beam,
    vector<Object> const& objects,
    vector<Sphere> const& spheres,
    vector<ReflectiveShadowMap> const& shadowMaps,
    vector<Transformer> const& transformers
) {
    size_t n = beam.size();
    for (size_t i = 0; i < n; i++) {
        bool passedToInfinity = false;
        while (accuracyNotReached(beam[i]) && !passedToInfinity) {
            auto searchResPtr = findNextCollision(beam[i], objects, spheres);
            if (searchResPtr) {
                Collision collision = searchResPtr->first;
                unsigned objIndex = searchResPtr->second;
                float brightness = calculateBrightness(collision, shadowMaps, transformers);
                collision.brightness = brightness;
                BeamSegment reflected = getReflectedSegment(beam[i], collision);
                BeamSegment transmitted = getTransmittedSegment(beam[i], collision, objects[objIndex]);
                beam[i] = reflected;
                beam.push_back(transmitted);
            } else
                passedToInfinity = true;
        }
    }
    return getColor(beam);
}

Color traceRay(
    Beam& beam,
    vector<Object> const& objects,
    vector<Sphere> const& spheres,
    vector<LightSource> const& sources
) {
    size_t n = beam.size();
    for (size_t i = 0; i < n; i++) {
        bool passedToInfinity = false;
        while (accuracyNotReached(beam[i]) && !passedToInfinity) {
            auto searchResPtr = findNextCollision(beam[i], objects, spheres);
            if (searchResPtr) {
                Collision collision = searchResPtr->first;
                unsigned objIndex = searchResPtr->second;
                float brightness = traceBrightness(collision, sources, objects, spheres);
                collision.brightness = brightness;
                BeamSegment reflected = getReflectedSegment(beam[i], collision);
                BeamSegment transmitted = getTransmittedSegment(beam[i], collision, objects[objIndex]);
                beam[i] = reflected;
                beam.push_back(transmitted);
            } else
                passedToInfinity = true;
        }
    }
    return getColor(beam);
}
