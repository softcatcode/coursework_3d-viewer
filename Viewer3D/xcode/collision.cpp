//
//  collision.cpp
//  Viewer3D
//
//  Created by Daniil on 27.11.2023.
//

#include "collision.hpp"
#include "mathutils.hpp"
#include <algorithm>

bool collisionCmp(Collision const& a, Collision const& b)
{
    if (abs(a.dist - b.dist) > EPS)
        return a.dist < b.dist;
    return a.n.z < b.n.z;
}

unsigned nearestCollisionIndex(vector<Collision> const& collisions, vec3 const& point)
{
    float minDist = 1e9;
    size_t n = collisions.size(), result;
    for (size_t i = 0; i < n; i++) {
        vec3 v = collisions[i].point() - point;
        float len = length(v);
        if (minDist > len) {
            minDist = len;
            result = i;
        }
    }
    return result;
}

Collision const& nearestCollision(vector<Collision> const& collisions, vec3 const& point)
{
    unsigned index = nearestCollisionIndex(collisions, point);
    return collisions[index];
}
