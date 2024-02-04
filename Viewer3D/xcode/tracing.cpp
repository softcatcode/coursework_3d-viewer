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

pair<BeamSegment, BeamSegment> split(BeamSegment const& beamSeg, Collision const& collision, Object const& obj)
{
    Ray const& ray = beamSeg.ray;
    vec3 const& n = collision.n;
    vec3 const& point = collision.point();
    float k = collision.optDensity();
    float t = collision.transmission(), r = collision.reflection();
    
    Ray reflRay = reflectedRay(ray, point, n);
    Ray transRay = transmittedRay(ray, point, n, k);
    Color transLight = collision.color * t;
    Color reflLight = min(collision.color - transLight, obj.getColor());
    
    BeamSegment reflected = {
        reflRay,
        beamSeg.color + reflLight * beamSeg.nextReflectionWeight,
        beamSeg.collisionCount + 1,
        .nextReflectionWeight = beamSeg.nextReflectionWeight * r
    };
    BeamSegment transmitted = {
        transRay,
        transLight,
        beamSeg.collisionCount + 1,
        .nextReflectionWeight = t
    };
    return { reflected, transmitted };
}

Color traceBeam(
    Beam& beam,
    vector<Object> const& objects,
    vector<Sphere> const& spheres,
    BrightnessCalcArgs& brCalcArgs
) {
    size_t n = beam.size();
    for (size_t i = 0; i < n; i++) {
        bool passedToInfinity = false;
        while (accuracyNotReached(beam[i]) && !passedToInfinity) {
            auto searchResPtr = findNextCollision(beam[i], objects, spheres);
            if (searchResPtr) {
                Collision collision = searchResPtr->first;
                unsigned objIndex = searchResPtr->second;
                switch (brCalcArgs.selection) {
                    case BrightnessCalcMethod::rsm:
                        brCalcArgs.methodRSMArgs.collision = collision;
                        break;
                    case BrightnessCalcMethod::tracing:
                        brCalcArgs.methodTracingArgs.collision = collision;
                        break;
                    case BrightnessCalcMethod::fong:
                        brCalcArgs.methodFongArgs.collision = collision;
                        brCalcArgs.methodFongArgs.rayDir = beam[i].ray.getDirection();
                        break;
                }
                collision.color = calculateBrightness(brCalcArgs);
                auto segments = split(beam[i], collision, objects[objIndex]);
                beam[i] = segments.first;
                beam.push_back(segments.second);
            } else
                passedToInfinity = true;
        }
    }
    return getColor(beam);
}
