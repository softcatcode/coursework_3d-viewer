//
//  operationmanager.cpp
//  Viewer3D
//
//  Created by Daniil on 24.11.2023.
//

#include "operationmanager.hpp"
#include "ioutils.hpp"
#include "drawutils.hpp"
#include "raytracer.hpp"
#include "parameters.hpp"

void execute(DrawAlgoData& data, int request)
{
    switch (request)
    {
        case READ_STAGE:
            readStage(data.stage, "stageData.txt");
            break;
        case INIT_TRACER:
            initRayTracer(
                data.tracer,
                data.stage,
                data.imgWidth,
                data.imgHeight,
                BrightnessCalcMethod::tracing
            );
            break;
        case TRACE_RAYS:
            data.image = trace(data.tracer, data.step);
            data.needsUpdate = false;
            break;
        case DRAW_GENERATED_IMG:
            drawImage(data.image);
            break;
        case CLEAR_DRAW_DATA:
            gl::clear(backgroundColor);
            break;
        case UPDATE_STEP:
            data.step = getNewDrawStep(data.step);
            break;
    }
}
