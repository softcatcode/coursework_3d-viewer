//
//  shadowmap.hpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#ifndef shadowmap_hpp
#define shadowmap_hpp

#define sizeOf(a) ivec2(a[0].size(), a.size());

#include <vector>
#include "rsmelem.hpp"
#include "stageobj.hpp"
using namespace std;

using ReflectiveShadowMap = vector< vector<ReflectiveShadowMapElem> >;

vector<ReflectiveShadowMap> buildShadowMaps(
    vector<Object> const& objects,
    vector<Transformer> const& transformers,
    vector<LightSource> const& sources,
    unsigned width, unsigned height
);

void updateShadowMapByTriangle(
    ReflectiveShadowMap& shadowMap,
    Triangle const& tr,
    mat3& points,
    ObjectProperties const& objProp
);

void updateShadowMap(ReflectiveShadowMap& shadowMap, Object const& object);

#endif /* shadowmap_hpp */
