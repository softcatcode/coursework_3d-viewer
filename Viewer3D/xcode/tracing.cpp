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
    vec3 point = collision.point();
    vec3 reflDir = reflectionDir(beamSeg.ray.getDirection(), collision.n);
    Ray transRay = transmittedRay(beamSeg.ray, obj, point, collision.n, collision.optDensity());
    Color reflColor = min(collision.color, obj.getColor());
    Color transColor;
    if (beamSeg.collisionCount > 0) {
        reflColor = min(beamSeg.color, reflColor);
        transColor = (beamSeg.color - reflColor) * collision.transmission();
    } else
        transColor = (collision.color - reflColor) * collision.transmission();
    BeamSegment reflected = { Ray(point + (0.01f * reflDir), reflDir), reflColor, beamSeg.collisionCount + 1 };
    BeamSegment transmitted = { transRay, transColor, beamSeg.collisionCount + 1 };
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
