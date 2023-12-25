//
//  collision.hpp
//  Viewer3D
//
//  Created by Daniil on 27.11.2023.
//

#ifndef collision_hpp
#define collision_hpp

#include "objproperties.hpp"
#include "parameters.hpp"
#include <vector>
using namespace cinder;
using namespace std;

struct Collision
{
    vec3 n;
    ObjectProperties objProp;
    float x, y;
    union {
        float dist, z;
    };
    Color color = backgroundColor;
    float delta = 0.f;
    
    vec3 point() const { return vec3(x, y, dist); }
    float optDensity() const { return objProp.optDensity; }
    float reflection() const { return objProp.reflection; }
    float transmission() const { return objProp.transmission; }
    float diffuse() const { return objProp.diffuseRefl; }
    float shineRatio() const { return objProp.shineRatio; }
};

// used only when observing in z axis direction
bool collisionCmp(Collision const& a, Collision const& b);

unsigned nearestCollisionIndex(vector<Collision> const& collisions, vec3 const& point);

Collision const& nearestCollision(vector<Collision> const& collisions, vec3 const& point);

#endif /* collision_hpp */
