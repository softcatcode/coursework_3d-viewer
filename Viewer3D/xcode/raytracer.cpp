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
    if (method == BrightnessCalcMethod::rsm) {
        tracer.sourceMaps = buildShadowMaps(
            tracer.objects, tracer.transformers, stage.sources, imgWidth, imgHeight
        );
        tracer.transformers = getSourceTransformers(stage.sources);
    } else if (method == BrightnessCalcMethod::tracing) {
        tracer.sources = stage.sources;
        tracer.cameraLocation = stage.camera.getLocation()
    }
    log("initRayTracer");
}

Color trace(RayTracer& tracer, unsigned i, unsigned j)
{
    BrightnessCalcArgs args;
    switch (tracer.method) {
        case BrightnessCalcMethod::rsm:
            args.methodRSMArgs.putArg2(tracer.sourceMaps);
            args.methodRSMArgs.putArg3(tracer.transformers);
            break;
        case BrightnessCalcMethod::tracing:
            args.methodTracingArgs.putArg2(tracer.sources);
            args.methodTracingArgs.putArg3(tracer.objects);
            args.methodTracingArgs.putArg4(tracer.spheres);
            break;
    }
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
