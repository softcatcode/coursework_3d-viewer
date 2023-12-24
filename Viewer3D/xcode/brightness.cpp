//
//  brightness.cpp
//  Viewer3D
//
//  Created by Daniil on 14.12.2023.
//

#include "brightness.hpp"
#include "beam.hpp"
#include "parameters.hpp"
using namespace cinder;

Color reflectionBrightness(vec3 point, vec3 n, ReflectiveShadowMapElem const& rsmElem)
{
    Color result(0.f, 0.f, 0.f);
    for (auto const& collision: rsmElem) {
        vec3 w = collision.point() - point;
        Color brVal = collision.color * collision.diffuse();
        brVal *= max(0.f, dot(collision.n, w));
        brVal *= max(0.f, dot(collision.n, -w));
        float sqrLen = (w.x * w.x + w.y * w.y + w.z * w.z);
        brVal /= sqrLen;
        result += brVal;
    }
    return result;
}

Color directBrightness(vec3 point, ReflectiveShadowMap const& shadowMap, Transformer const& transformer)
{
    transformPoint(point, transformer);
    auto mapSize = sizeOf(shadowMap);
    float result = 0.f;
    if (
        0 <= point.y && point.y < mapSize.y &&
        0 <= point.x && point.x <= mapSize.x
    ) {
        unsigned short i = point.y, j = point.x;
        auto const& rsmElem = shadowMap[i][j];
        size_t collisionCount = rsmElem.size(), k = 0;
        while (k < collisionCount && abs(point.z - rsmElem[k].dist) >= rsmElem[k].delta)
            ++k;
        if (k < collisionCount)
            result = rsmElem[k].color;
    }
    return result;
}

ivec2 getRandomElemPosition(ivec2 mapSize)
{
    unsigned x = rand() % unsigned(mapSize.x);
    unsigned y = rand() % unsigned(mapSize.y);
    return ivec2(x, y);
}

Color rsmBrightness(
    Collision const& collision,
    vector<ReflectiveShadowMap> const& shadowMaps,
    vector<Transformer> const& transformers
) {
    Color brightness(0.f, 0.f, 0.f);
    size_t n = shadowMaps.size();
    vec3 point = collision.point();
    for (size_t i = 0; i < n; ++i) {
        brightness += directBrightness(point, shadowMaps[i], transformers[i]);
        auto mapSize = sizeOf(shadowMaps[i]);
        for (unsigned j = 0; j < SAMPLE_SIZE; ++j) {
            ivec2 position = getRandomElemPosition(mapSize);
            auto rsmElem = shadowMaps[i][unsigned(position.y)][unsigned(position.x)];
            brightness += reflectionBrightness(point, collision.n, rsmElem);
        }
    }
    return brightness;
}

Color traceBrightness(
    Collision const& collision,
    vector<LightSource> const& sources,
    vector<Object> const& objects,
    vector<Sphere> const& spheres
) {
    vec3 point = collision.point();
    Color result(0.f, 0.f, 0.f);
    for (auto const& source: sources) {
        float k = collision.diffuse(), passedDist = 0.f;
        vec3 srcVec = source.location - point;
        vec3 srcDir = normalize(srcVec);
        float srcDist = length(srcVec);
        BeamSegment beam = { Ray(point + 0.1f * srcDir, srcDir), backgroundColor, source.power, 0 };
        bool nextCollisionExists = true;
        while (nextCollisionExists) {
            unique_ptr< pair<Collision, unsigned> > searchResPtr = findNextCollision(beam, objects, spheres);
            if (searchResPtr) {
                float collisionDist = searchResPtr->first.dist;
                passedDist += collisionDist;
                if (passedDist < srcDist) {
                    beam.ray.setOrigin(beam.ray.calcPosition(collisionDist + 0.1f));
                    k *= searchResPtr->first.transmission();
                } else
                    nextCollisionExists = false;
            }
            else
                nextCollisionExists = false;
        }
        result += k * source.color / length(srcVec);
    }
    return result;
}

Color calculateBightness(BrightnessCalcArgs const& args)
{
    Color result;
    switch (args.selection) {
        case BrightnessCalcMethod::rsm:
            result = rsmBrightness(
                args.methodRSMArgs.arg1(),
                args.methodRSMArgs.arg2(),
                args.methodRSMArgs.arg3()
            );
            break;
        case BrightnessCalcMethod::tracing:
            result = tracingBrightness(
                args.methodTracingArgs.arg1(),
                args.methodTracingArgs.arg2(),
                args.methodTracingArgs.arg3(),
                args.methodTracingArgs.arg4()
            );
            break;
    }
}
