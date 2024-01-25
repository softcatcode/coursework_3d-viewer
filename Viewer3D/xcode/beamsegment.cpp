//
//  raysegment.cpp
//  Viewer3D
//
//  Created by Daniil on 26.11.2023.
//

#include "parameters.hpp"
#include "beamsegment.hpp"
#include "mathutils.hpp"
#include <vector>
#include <algorithm>
#include <memory>
using namespace std;

float colorPower(Color const&  color)
{
    return color.r + color.g + color.b;
}

bool accuracyNotReached(BeamSegment const& seg)
{
    return seg.collisionCount < MAX_REFL_COUNT;
}

unique_ptr<Collision> rayObjectCollision(Ray const& ray, Object const& object)
{
    vector<Collision> collisions;
    size_t n = object.triangles.size();
    for (size_t i = 0; i < n; ++i) {
        mat3 points = object.trianglePoints(i);
        float dist;
        float projection = dot(ray.getDirection(), object.triangleNorm(i));
        if (abs(projection) > EPS && ray.calcTriangleIntersection(points[0], points[1], points[2], &dist)) {
            if (dist > 0) {
                vec3 point = ray.calcPosition(dist);
                Collision collision = {
                    object.triangleNorm(i), object.properties, point.x, point.y, point.z
                };
                collisions.push_back(collision);
            }
        }
    }
    
    unique_ptr<Collision> result;
    if (!collisions.empty())
        result = make_unique<Collision>(nearestCollision(collisions, ray.getOrigin()));
    return result;
}

unique_ptr< pair<Collision, unsigned> > findNextCollision(
    BeamSegment beamSeg,
    vector<Object> const& objects,
    vector<Sphere> const& spheres
) {
    vector<Collision> collisions;
    vector<unsigned> objIndexes; // storing indexes i of objects which correspond collisions[i]
    unsigned n = (unsigned)objects.size();
    vector<bool> visited(n, false);
    float passedDist = 0.f;
    vec3 initLocation = beamSeg.ray.getOrigin();
    
    while (passedDist < MAX_DIST) {
        float minDist = MAX_DIST;
        for (unsigned i = 0; i < n; ++i) {
            if (!visited[i]) {
                float dist = length(spheres[i].getCenter() - beamSeg.ray.getOrigin()) - spheres[i].getRadius();
                if (dist >= 1.f) {
                    minDist = min(minDist, dist);
                } else {
                    unique_ptr<Collision> collision = rayObjectCollision(beamSeg.ray, objects[i]);
                    if (collision) {
                        collisions.push_back(*collision);
                        objIndexes.push_back(i);
                    }
                    visited[i] = true;
                }
            }
        }
        beamSeg.ray.setOrigin(beamSeg.ray.calcPosition(minDist));
        passedDist += minDist;
    }
    
    unique_ptr< pair<Collision, unsigned> > result;
    if (!collisions.empty()) {
        unsigned index = nearestCollisionIndex(collisions, initLocation);
        result = make_unique<pair<Collision, unsigned>>(
            make_pair(collisions[index], objIndexes[index])
        );
    }
    return result;
}

Ray transmittedRay(
    Ray const& ray,
    vec3 const& point,
    vec3 const& n,
    float optDensity
) {
    vec3 dir = afterTransitionDir(ray.getDirection(), n, spaceOptDensity, optDensity);
    Ray result(point + 0.01f * dir, dir);
    return result;
}

Ray reflectedRay(Ray const& ray, vec3 const& point, vec3 const& n)
{
    vec3 dir = reflectionDir(ray.getDirection(), n);
    Ray result(point + 0.01f * dir, dir);
    return result;
}
