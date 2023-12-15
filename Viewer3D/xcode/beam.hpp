//
//  beam.hpp
//  Viewer3D
//
//  Created by Daniil on 25.11.2023.
//

#ifndef beam_hpp
#define beam_hpp

#include "beamsegment.hpp"
#include "shadowmap.hpp"

using namespace std;
using namespace cinder;

using Beam = vector<BeamSegment>;

void initBeam(Beam& beam, vec3 const& point, vec3 const& direction);

Color getColor(Beam const& beam);

#endif /* beam_hpp */
