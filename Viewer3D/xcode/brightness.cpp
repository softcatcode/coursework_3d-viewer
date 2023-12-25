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
        float sqrLen = w.x * w.x + w.y * w.y + w.z * w.z;
        brVal /= sqrLen;
        result += brVal;
    }
    return result;
}

Color directBrightness(vec3 point, ReflectiveShadowMap const& shadowMap, Transformer const& transformer)
{
    transformPoint(point, transformer);
    auto mapSize = sizeOf(shadowMap);
    Color result = Color(0.f, 0.f, 0.f);
    if (
        0 <= point.y && point.y < mapSize.y &&
        0 <= point.x && point.x <= mapSize.x
    ) {
        uint16_t i = uint16_t(point.y), j = uint16_t(point.x);
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
    unsigned x = unsigned(rand()) % unsigned(mapSize.x);
    unsigned y = unsigned(rand()) % unsigned(mapSize.y);
    return ivec2(x, y);
}

Color rsmBrightness(
    Collision const& collision,
    vector<ReflectiveShadowMap> const& shadowMaps,
    vector<Transformer> const& transformers
) {
    Color result(0.f, 0.f, 0.f);
    size_t n = shadowMaps.size();
    vec3 point = collision.point();
    for (size_t i = 0; i < n; ++i) {
        result += directBrightness(point, shadowMaps[i], transformers[i]);
        auto mapSize = sizeOf(shadowMaps[i]);
        for (unsigned j = 0; j < SAMPLE_SIZE; ++j) {
            ivec2 position = getRandomElemPosition(mapSize);
            auto rsmElem = shadowMaps[i][position.y][position.x];
            result += reflectionBrightness(point, collision.n, rsmElem);
        }
    }
    return result;
}

Color sourceLight(
    LightSource const& source,
    vec3 const& point,
    float diffuseRatio,
    vector<Object> const& objects,
    vector<Sphere> const& spheres
) {
    float k = diffuseRatio, passedDist = 0.f;
    vec3 srcVec = source.location - point;
    vec3 srcDir = normalize(srcVec);
    float srcDist = length(srcVec);
    BeamSegment beam = { Ray(point + 0.1f * srcDir, srcDir), source.color, 0 };
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
    return k * source.color / length(srcVec);
}

Color traceBrightness(
    Collision const& collision,
    vector<LightSource> const& sources,
    vector<Object> const& objects,
    vector<Sphere> const& spheres
) {
    vec3 point = collision.point();
    float d = collision.diffuse();
    Color result(0.f, 0.f, 0.f);
    for (auto const& src: sources)
        result += sourceLight(src, point, d, objects, spheres);
    return result;
}

Color fongBrightness(
    Collision const& collision,
    vector<LightSource> const& sources,
    vector<Object> const& objects,
    vector<Sphere> const& spheres,
    vec3 const& cameraLocation,
    vec3 const& rayDir
) {
    vec3 n = collision.n;
    vec3 point = collision.point();
    vec3 camDir = normalize(cameraLocation - point);
    vec3 reflDir = reflectionDir(rayDir, n);
    float d = collision.diffuse();
    float r = collision.reflection();
    Color result = backgroundLight;
    for (auto const& src: sources) {
        vec3 srcDir = normalize(src.location - point);
        Color light = sourceLight(src, point, d, objects, spheres);
        result += d * dot(srcDir, n) * light;
        result += r * dot(reflDir, camDir) * light;
    }
    return result;
}

Color calculateBrightness(BrightnessCalcArgs const& args)
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
            result = traceBrightness(
                args.methodTracingArgs.arg1(),
                args.methodTracingArgs.arg2(),
                args.methodTracingArgs.arg3(),
                args.methodTracingArgs.arg4()
            );
            break;
        case BrightnessCalcMethod::fong:
            result = fongBrightness(
                args.methodFongArgs.arg1(),
                args.methodFongArgs.arg2(),
                args.methodFongArgs.arg3(),
                args.methodFongArgs.arg4(),
                args.methodFongArgs.arg5(),
                args.methodFongArgs.arg6()
            );
            break;
    }
    return result;
}
