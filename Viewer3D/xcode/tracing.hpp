//
//  tracing.hpp
//  Viewer3D
//
//  Created by Daniil on 14.12.2023.
//

#ifndef tracing_hpp
#define tracing_hpp

#include <vector>
#include "brightness.hpp"
#include "shadowmap.hpp"
#include "mathutils.hpp"
#include "beam.hpp"
using namespace std;

Color traceBeam(
    Beam& beam,
    vector<Object> const& objects,
    vector<Sphere> const& spheres,
    BrightnessCalcArgs& brCalcArgs
);

#endif /* tracing_hpp */
