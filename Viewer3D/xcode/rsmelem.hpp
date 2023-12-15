//
//  rsmelem.hpp
//  Viewer3D
//
//  Created by Daniil on 24.11.2023.
//

#ifndef rsmelem_hpp
#define rsmelem_hpp

#define SAMPLE_SIZE 5u

#include "mathutils.hpp"
#include "collision.hpp"
#include "objproperties.hpp"
using namespace cinder;
using namespace std;

using ReflectiveShadowMapElem = vector<Collision>;

void updateShadowMapElem(
    ReflectiveShadowMapElem& rsmElem,
    float x, float y,
    Triangle const& triangle,
    ObjectProperties const& objProp
);

void sortRSMElemData(ReflectiveShadowMapElem& rsmElem);

void deleteDuplicates(ReflectiveShadowMapElem& rsmElem);

void calcBrightnessForRSMElem(ReflectiveShadowMapElem& elem, float power);

#endif /* rsmelem_hpp */
