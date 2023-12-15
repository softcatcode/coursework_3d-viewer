//
//  raysegment.cpp
//  Viewer3D
//
//  Created by Daniil on 26.11.2023.
//

#include "parameters.hpp"
#include "beamsegment.hpp"
#include <vector>
#include <algorithm>
#include <memory>
using namespace std;

bool accuracyNotReached(BeamSegment const& seg)
{
    return seg.power > MIN_RAY_POWER && seg.collisionCount < MAX_REFL_COUNT;
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
                Collision collision = { object.triangleNorm(i), object.properties, point.x, point.y, point.z };
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

BeamSegment getReflectedSegment(BeamSegment const& beamSeg, Collision const& collision) {
    vec3 dir = reflectionDir(beamSeg.ray.getDirection(), collision.n);
    vec3 point = collision.point() + (0.1f * dir);
    float power = beamSeg.power * collision.brightness;
    Color color;
    if (beamSeg.collisionCount > 0)
        color = colorAfterCollision(beamSeg.color, collision.color(), power);
    else
        color = collision.color() * power;
    power *= collision.reflection();
    return { Ray(point, dir), color, power, beamSeg.collisionCount + 1 };
}

BeamSegment getTransmittedSegment(
    BeamSegment const& beamSeg,
    Collision const& collision,
    Object const& object
) {
    float materialOptDensity = collision.optDensity();
    vec3 dir = afterTransitionDir(
        beamSeg.ray.getDirection(), collision.n, spaceOptDensity, materialOptDensity
    );
    Ray ray(collision.point() + (0.01f * dir), dir);
    float power = beamSeg.power * collision.brightness;
    Color color;
    if (beamSeg.collisionCount > 0)
        color = colorAfterCollision(beamSeg.color, collision.color(), power);
    else
        color = collision.color() * power;
    power *= collision.transmission();
    
    auto nextCol = rayObjectCollision(ray, object);
    BeamSegment result;
    if (nextCol)
    {
        vec3 finalDir = afterTransitionDir(
            ray.getDirection(), nextCol->n, materialOptDensity, spaceOptDensity
        );
        Color finalColor = colorAfterCollision(color, nextCol->color(), power);
        Ray finalRay(nextCol->point() + (0.1f * finalDir), finalDir);
        result = { finalRay, finalColor, power, beamSeg.collisionCount + 1 };
    }
    else
        result = { ray, color, power, beamSeg.collisionCount + 1 };
    return result;
}
