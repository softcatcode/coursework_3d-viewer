//
//  raysegment.hpp
//  Viewer3D
//
//  Created by Daniil on 26.11.2023.
//

#ifndef beamsegment_hpp
#define beamsegment_hpp

#include <vector>
#include <memory>
#include "stageobj.hpp"
#include "collision.hpp"
#include "shadowmap.hpp"

using namespace cinder;
using namespace std;

struct BeamSegment
{
    Ray ray;
    Color color;
    unsigned collisionCount;
};

unique_ptr< pair<Collision, unsigned> > findNextCollision(
    BeamSegment beamSeg,
    vector<Object> const& objects,
    vector<Sphere> const& spheres
);

bool accuracyNotReached(BeamSegment const& seg);

int selectNearestCollision(vec3 const& location, vector<Collision> const& collisions);

unique_ptr< pair<Collision, unsigned> > findNextCollision(
    BeamSegment beamSeg,
    vector<Object> const& objects,
    vector<Sphere> const& spheres
);

unique_ptr<Collision> rayObjectCollision(Ray const& ray, Object const& object);

BeamSegment getReflectedSegment(BeamSegment const& beamSeg, Collision const& collision);

BeamSegment getTransmittedSegment(
    BeamSegment const& beamSeg,
    Collision const& collision,
    Object const& object
);

#endif /* beamsegment_hpp */
