//
//  stageobj.cpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#include "stageobj.hpp"
#include "mathutils.hpp"
#include <memory>
using namespace std;

vector<Transformer> getSourceTransformers(vector<LightSource> const& sources)
{
    size_t n = sources.size();
    vector<Transformer> result(n);
    for (size_t i = 0; i < n; i++)
        result[i] = buildTransformer(sources[i].location, sources[i].x, sources[i].y, sources[i].z);
    return result;
}

void transformObject(Object& object, Transformer const& transformer)
{
    for (auto& point: object.points)
        transformPoint(point, transformer);
    for (auto& triangle: object.triangles)
        transformVector(triangle.n, transformer);
    buildPlanesForTriangles(object.triangles, object.points);
}

void transformSource(LightSource& source, Transformer const& transformer)
{
    transformPoint(source.location, transformer);
    transformVector(source.x, transformer);
    transformVector(source.y, transformer);
    transformVector(source.z, transformer);
}

void transformCamera(Observer& camera, Transformer const& transformer)
{
    transformPoint(camera.location, transformer);
    transformVector(camera.x, transformer);
    transformVector(camera.y, transformer);
    transformVector(camera.z, transformer);
}

Sphere buildSphere(Object const& object)
{
    vec3 center(0.f, 0.f, 0.f);
    for (vec3 point: object.points)
        center += point;
    center /= float(object.points.size());
    float r;
    for (vec3 point: object.points) {
        vec3 dist = point - center;
        r = max(r, length(dist));
    }
    return Sphere(center, r);
}

void buildPlanesForTriangles(
    vector<Triangle>& triangles,
    vector<vec3> const& points
) {
    for (auto& triangle: triangles) {
        vec4 ratios = vec4(triangle.n, -1.f * dot(triangle.n, points[triangle.a]));
        triangle.setPlane(ratios);
    }
}

void translateCamera(Observer& camera, vec3 step)
{
    camera.location = camera.location + step.x * camera.x + step.y * camera.y + step.z * camera.z;
}

void rotateCamera(Observer& camera, char axis, float angle)
{
    if (axis == 'x') {
        mat3 m = rotate(angle, camera.x);
        camera.y = camera.y * m;
        camera.z = camera.z * m;
    } else if (axis == 'y') {
        mat3 m = rotate(angle, camera.y);
        camera.x = camera.x * m;
        camera.z = camera.z * m;
    } else if (axis == 'z') {
        mat3 m = rotate(unit, camera.z);
        camera.x = camera.x * m;
        camera.y = camera.y * m;
    }
}

vector<Object> transformedObjects(vector<Object> const& objects, Transformer const& transformer)
{
    vector<Object> result = objects;
    for (auto& obj: result)
        transformObject(obj, transformer);
    return result;
}

mat3 Object::trianglePoints(size_t i) const
{
    Triangle const& tr = triangles[i];
    return mat3(points[tr.a], points[tr.b], points[tr.c]);
}

vec3 Object::triangleNorm(size_t i) const
{
    return triangles[i].n;
}
