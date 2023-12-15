//
//  interfacemanager.hpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#ifndef interfacemanager_hpp
#define interfacemanager_hpp

#include "viewer3d.hpp"

#define SETUP_REQUEST 1
#define UPDATE_REQUEST 2
#define DRAW_REQUEST 3

void processRequest(Viewer3DApp const& app, int request);

#endif /* interfacemanager_hpp */
