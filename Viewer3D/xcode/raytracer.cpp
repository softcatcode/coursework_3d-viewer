//
//  raytracer.cpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#include "raytracer.hpp"
#include "drawutils.hpp"
#include "parameters.hpp"
#include "tracing.hpp"
#include "logs.hpp"

void initTracerBeams(RayTracer& tracer, Observer const& camera, unsigned imgWidth, unsigned imgHeight)
{
    tracer.imgWidth = imgWidth;
    tracer.imgHeight = imgHeight;
    resize(tracer.data, imgHeight, imgWidth);
    
    for (unsigned i = 0; i < imgHeight; ++i) {
        for (unsigned j = 0; j < imgWidth; ++j) {
            vec3 point = camera.location + (float(j) * camera.x) + (float(imgHeight - i) * camera.y);
            initBeam(tracer.data[i][j], point, camera.z);
        }
    }
    log("initTracerBeams");
}

void initRayTracer(
    RayTracer& tracer,
    Stage const& stage,
    unsigned imgWidth,
    unsigned imgHeight,
    BrightnessCalcMethod method
) {
    tracer.objects = stage.objects;
    tracer.spheres = buildSpheres(tracer.objects);
    initTracerBeams(tracer, stage.camera, imgWidth, imgHeight);
    tracer.imgWidth = imgWidth;
    tracer.imgHeight = imgHeight;
    tracer.method = method;
    if (method == BrightnessCalcMethod::rsm) {
        tracer.sourceMaps = buildShadowMaps(
            tracer.objects, tracer.transformers, stage.sources, imgWidth, imgHeight
        );
        tracer.transformers = getSourceTransformers(stage.sources);
    } else if (
        method == BrightnessCalcMethod::tracing ||
        method == BrightnessCalcMethod::fong
    ) {
        tracer.sources = stage.sources;
        tracer.cameraLocation = stage.camera.getLocation();
    }
    log("initRayTracer");
}

BrightnessCalcArgs getBrightnessCalcArgsStruct(RayTracer& tracer)
{
    switch (tracer.method) {
        case BrightnessCalcMethod::rsm:
            return {
                tracer.method,
                {
                    .methodRSMArgs = {
                        .collision = {},
                        .shadowMaps = tracer.sourceMaps,
                        .transformers = tracer.transformers
                    }
                }
            };
        case BrightnessCalcMethod::tracing:
            return {
                tracer.method,
                {
                    .methodTracingArgs = {
                        .collision = {},
                        .sources = tracer.sources,
                        .objects = tracer.objects,
                        .spheres = tracer.spheres
                    }
                }
            };
        case BrightnessCalcMethod::fong:
            return {
                tracer.method,
                {
                    .methodFongArgs = {
                        .collision = {},
                        .sources = tracer.sources,
                        .objects = tracer.objects,
                        .spheres = tracer.spheres,
                        .cameraLocation = tracer.cameraLocation,
                        .rayDir = vec3(0.f, 0.f, 0.f)
                    }
                }
            };
    }
}

Color trace(RayTracer& tracer, unsigned i, unsigned j)
{
    BrightnessCalcArgs&& args = getBrightnessCalcArgsStruct(tracer);
    return traceBeam(tracer.data[i][j], tracer.objects, tracer.spheres, args);
}

Surface trace(RayTracer& tracer, unsigned step)
{
    unsigned width = (tracer.imgWidth + step - 1) / step;
    unsigned height = (tracer.imgHeight + step - 1) / step;
    Surface result((int32_t)width, (int32_t)height, false);
    auto iter = result.getIter();
    unsigned i = 0, j = 0;
    while (iter.line()) {
        while (iter.pixel()) {
            Color color = trace(tracer, i, j);
            iter.r() = (unsigned char) color.r;
            iter.g() = (unsigned char) color.g;
            iter.b() = (unsigned char) color.b;
            j += step;
        }
        i += step;
        j = 0;
    }
    result = enlarge(result, step);
    log("trace");
    return result;
}
