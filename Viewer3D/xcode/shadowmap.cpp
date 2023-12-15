//
//  shadowmaps.cpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#include "shadowmap.hpp"
#include "logs.hpp"

void updateShadowMap(ReflectiveShadowMap& shadowMap, Object const& object)
{
    size_t n = object.triangles.size();
    for (size_t i = 0; i < n; ++i) {
        mat3 points = object.trianglePoints(i);
        updateShadowMapByTriangle(shadowMap, object.triangles[i], points, object.properties);
    }
}

void updateRSMSubtask(
    ReflectiveShadowMap& shadowMap,
    Line2d const& a, Line2d const& b,
    int y1, int y2,
    Triangle const& triangle,
    ObjectProperties const& objProp
) {
    if (y1 > y2)
        swap(y1, y2);
    y1 = max(y1, 0);
    ivec2 ms = sizeOf(shadowMap);
    y2 = min(ms.y - 1, y2);
    for (int y = y1; y <= y2; ++y)
    {
        int x1 = crossPixel(a, y);
        int x2 = crossPixel(b, y);
        if (x1 > x2)
            swap(x1, x2);
        x2 = min(ms.x - 1, x2);
        float yf = float(y);
        x1 = max(x1, 0);
        for (int x = x1; x <= x2; ++x) {
            float xf = float(x);
            updateShadowMapElem(shadowMap[y][x], xf, yf, triangle, objProp);
        }
    }
}

void updateShadowMapByTriangle(
    ReflectiveShadowMap& shadowMap,
    Triangle const& tr,
    mat3& points,
    ObjectProperties const& objProp
) {
    vec3 p1 = points[0], p2 = points[1], p3 = points[2];
    sortByY(p1, p2, p3);
    Line2d a = buildLine(p1.x, p1.y, p3.x, p3.y);
    Line2d b = buildLine(p1.x, p1.y, p2.x, p2.y);
    Line2d c = buildLine(p2.x, p2.y, p3.x, p3.y);
    int y1 = p1.y, y2 = p2.y, y3 = p3.y;
    updateRSMSubtask(shadowMap, a, b, y1, y2, tr, objProp);
    updateRSMSubtask(shadowMap, a, c, y2 + 1, y3, tr, objProp);
}

vector<ReflectiveShadowMap> buildShadowMaps(
    vector<Object> const& objects,
    vector<Transformer> const& transformers,
    vector<LightSource> const& sources,
    unsigned imgWidth, unsigned imgHeight
) {
    size_t n = transformers.size();
    vector<ReflectiveShadowMap> shadowMaps(n);
    for (auto& map: shadowMaps)
        resize(map, imgHeight, imgWidth);
    
    for (size_t i = 0; i < n; i++) {
        vector<Object> transObjects = transformedObjects(objects, transformers[i]);
        for (auto const& object: transObjects)
            updateShadowMap(shadowMaps[i], object);
    }
    
    n = shadowMaps.size();
    for (size_t k = 0; k < n; ++k) {
        ReflectiveShadowMap& map = shadowMaps[k];
        size_t h = map.size();
        size_t w = map[0].size();
        for (size_t i = 0; i < h; ++i) {
            for (size_t j = 0; j < w; ++j) {
                sortRSMElemData(map[i][j]);
                deleteDuplicates(map[i][j]);
                calcBrightnessForRSMElem(map[i][j], sources[k].power);
            }
        }
    }
    log("buildShadowMaps");
    return shadowMaps;
}
