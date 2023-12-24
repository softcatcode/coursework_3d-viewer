//
//  brightness.hpp
//  Viewer3D
//
//  Created by Daniil on 14.12.2023.
//

#ifndef brightness_hpp
#define brightness_hpp

#include <vector>
#include "shadowmap.hpp"
#include "mathutils.hpp"
using namespace std;

enum BrightnessCalcMethod
{
    rsm, tracing;
};

union BrightnessCalcArgs
{
    BrightnessCalcMethod selection;
    
    struct
    {
        Collision const& collision;
        vector<ReflectiveShadowMap> const& shadowMaps;
        vector<Transformer> const& transformers;
        
        Collision const& arg1() const { return collision; }
        vector<ReflectiveShadowMap> const& arg2() const { return shadowMaps; }
        vector<Transformer> const& arg3() const { return transformers; }
        
        void putArg1(Collision const& value) { collision = value; }
        void putArg2(vector<ReflectiveShadowMap> const& value) { shadowMaps = value; }
        void putArg3(vector<Transformer> const& value) { transformers = value; }
    } methodRSMArgs;
    
    struct
    {
        Collision const& collision;
        vector<LightSource> const& sources;
        vector<Object> const& objects;
        vector<Sphere> const& spheres;
        
        Collision const& arg1() const { return collision; }
        vector<LightSource> const& arg2() const { return sources; }
        vector<Object> const& arg3() const { return objects; }
        vector<Sphere> const& arg4() const { return spheres; }
        
        void putArg1(Collision const& value) { collision = value; }
        void putArg2(vector<LightSource> const& value) { sources = value; }
        void putArg3(vector<Object> const& value) { objects = value; }
        void putArg4(vector<Sphere> const& value) { spheres = value; }
    } methodTracingArgs;
};

Color calculateBightness(BrightnessCalcArgs const& args);

#endif /* brightness_hpp */
