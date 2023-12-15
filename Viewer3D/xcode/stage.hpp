//
//  stage.hpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#ifndef stage_hpp
#define stage_hpp

#include "stageobj.hpp"
using namespace std;

struct Stage
{
    vector<Object> objects;
    vector<LightSource> sources;
    Observer camera;
};

void transformStage(Stage& objects, Transformer const& transformer);

vector<Sphere> buildSpheres(vector<Object> const& objects);

#endif /* stage_hpp */
