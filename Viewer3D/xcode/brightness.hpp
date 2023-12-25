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
    rsm, tracing, fong
};

struct BrightnessCalcArgs
{
    BrightnessCalcMethod selection;
    
    union
    {
        struct
        {
            Collision collision;
            vector<ReflectiveShadowMap>& shadowMaps;
            vector<Transformer>& transformers;
            
            Collision const& arg1() const { return collision; }
            vector<ReflectiveShadowMap> const& arg2() const { return shadowMaps; }
            vector<Transformer> const& arg3() const { return transformers; }
        } methodRSMArgs;
        
        struct
        {
            Collision collision;
            vector<LightSource>& sources;
            vector<Object>& objects;
            vector<Sphere>& spheres;
            
            Collision const& arg1() const { return collision; }
            vector<LightSource> const& arg2() const { return sources; }
            vector<Object> const& arg3() const { return objects; }
            vector<Sphere> const& arg4() const { return spheres; }
        } methodTracingArgs;
        
        struct
        {
            Collision collision;
            vector<LightSource>& sources;
            vector<Object>& objects;
            vector<Sphere>& spheres;
            vec3 cameraLocation, rayDir;
            
            Collision const& arg1() const { return collision; }
            vector<LightSource> const& arg2() const { return sources; }
            vector<Object> const& arg3() const { return objects; }
            vector<Sphere> const& arg4() const { return spheres; }
            vec3 const& arg5() const { return cameraLocation; }
            vec3 const& arg6() const { return rayDir; }
            
            void putArg1(Collision& value) { collision = value; }
            void putArg2(vector<LightSource>& value) { sources = value; }
            void putArg3(vector<Object>& value) { objects = value; }
            void putArg4(vector<Sphere>& value) { spheres = value; }
            void putArg5(vec3 const& value) { cameraLocation = value; }
            void putArg6(vec3 const& value) { rayDir = value; }
        } methodFongArgs;
    };
};

Color calculateBrightness(BrightnessCalcArgs const& args);

#endif /* brightness_hpp */
