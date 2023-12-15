//
//  stage.cpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#include "stage.hpp"

void transformStage(Stage& stage, Transformer const& transformer)
{
    for (auto& object: stage.objects)
        transformObject(object, transformer);
    for (auto& source: stage.sources)
        transformSource(source, transformer);
    transformCamera(stage.camera, transformer);
}

vector<Sphere> buildSpheres(vector<Object> const& objects)
{
    size_t n = objects.size();
    vector<Sphere> result(n);
    for (size_t i = 0; i < n; i++)
        result[i] = buildSphere(objects[i]);
    return result;
}
