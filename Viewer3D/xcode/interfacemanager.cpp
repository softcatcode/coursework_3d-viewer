//
//  interfacemanager.cpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#include "interfacemanager.hpp"
#include "interfacemanagerimpl.hpp"

void processRequest(Viewer3DApp const& app, int request)
{
    switch (request)
    {
        case SETUP_REQUEST:
            setupRequest(app);
            break;
        case UPDATE_REQUEST:
            updateRequest(app);
            break;
        case DRAW_REQUEST:
            drawRequest(app);
            break;
    }
}
