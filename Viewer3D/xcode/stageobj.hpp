//
//  stageobj.hpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#ifndef stageobj_hpp
#define stageobj_hpp

#include "mathutils.hpp"
#include "objproperties.hpp"
#include <vector>
using namespace std;
using namespace cinder;

struct Object
{
    vector<Triangle> triangles;
    vector<vec3> points;
    ObjectProperties properties;
    
    mat3 trianglePoints(size_t i) const;
    vec3 triangleNorm(size_t i) const { return triangles[i].n; };
    Color getColor() const { return properties.color; }
};

struct Observer
{
    vec3 location, x, y, z;
    
    vec3 getLocation() const { return location; }
};

struct LightSource
{
    vec3 location, x, y, z;
    Color color;
};

vector<Transformer> getSourceTransformers(vector<LightSource> const& sources);

void transformObject(Object& object, Transformer const& transformer);

void transformSource(LightSource& source, Transformer const& transformer);

void transformCamera(Observer& camera, Transformer const& transformer);

void translateCamera(Observer& camera, vec3 step);

void rotateCamera(Observer& camera, char axis, float angle);

vector<Object> transformedObjects(vector<Object> const& objects, Transformer const& transformer);

Sphere buildSphere(Object const& object);

void buildPlanesForTriangles(
    vector<Triangle>& triangles,
    vector<vec3> const& points
);

#endif /* stageobj_hpp */
