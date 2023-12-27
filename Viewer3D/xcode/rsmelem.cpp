//
//  rsmelem.cpp
//  Viewer3D
//
//  Created by Daniil on 24.11.2023.
//

#include "rsmelem.hpp"
#include "mathutils.hpp"
#include <iostream>
using namespace std;

float getDepthDelta(vec3 const& n)
{
    return sqrt(1 / (n.z * n.z) - 1) * 2;
}

void updateShadowMapElem(
    ReflectiveShadowMapElem& rsmElem,
    float x, float y,
    Triangle const& triangle,
    ObjectProperties const& objProp
) {
    float dist = triangle.depthOf(x, y);
    if (dist > 0.f) {
        Collision collision = { triangle.n, objProp, x, y, dist, backgroundColor, getDepthDelta(triangle.n) };
        rsmElem.push_back(collision);
    }
}

void sortRSMElemData(ReflectiveShadowMapElem& rsmElem)
{
    sort(rsmElem.begin(), rsmElem.end(), collisionCmp);
}

void deleteDuplicates(ReflectiveShadowMapElem& rsmElem)
{
    auto begIter = rsmElem.begin();
    int i = 0;
    while (i < int(rsmElem.size()) - 1) {
        if (int(rsmElem[i].z) == int(rsmElem[i + 1].z))
            rsmElem.erase(begIter + i);
        else
            ++i;
    }
}

void calcBrightnessForRSMElem(ReflectiveShadowMapElem& elem, Color const& light)
{
    Color color = light;
    size_t n = elem.size();
    for (size_t i = 0; i < n; ++i) {
        elem[i].color = color;
        color *= elem[i].transmission();
    }
}
