//
//  drawalgodata.hpp
//  Viewer3D
//
//  Created by Daniil on 24.11.2023.
//

#ifndef drawalgodata_hpp
#define drawalgodata_hpp

#include "stage.hpp"
#include "raytracer.hpp"
#include "brightness.hpp"
using namespace cinder;

struct DrawAlgoData
{
    Stage stage;
    RayTracer tracer;
    unsigned imgWidth, imgHeight, step;
    Surface image;
    bool updateReq = true;
    BrightnessCalcMethod shadowingMethod = BrightnessCalcMethod::rsm;
    
    bool needsUpdate() const { return updateReq; }
    void resumeUpdates() { updateReq = true; }
    void stopUpdate() { updateReq = false; }
};

#endif /* drawalgodata_hpp */
