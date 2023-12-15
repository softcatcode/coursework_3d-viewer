//
//  drawutils.hpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#ifndef drawutils_hpp
#define drawutils_hpp

#include "shadowmap.hpp"
#include "stage.hpp"
#include <vector>
using namespace std;
using namespace cinder;

void drawImage(Surface const& image);

unsigned getNewDrawStep(unsigned step);

Surface enlarge(Surface const& img, unsigned k);

#endif /* drawutils_hpp */
