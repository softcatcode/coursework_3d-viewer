//
//  interfacemanagerimpl.cpp
//  Viewer3D
//
//  Created by Daniil on 24.11.2023.
//

#include "interfacemanagerimpl.hpp"
#include "operationmanager.hpp"

void setupRequest(Viewer3DApp const& app)
{
    execute(app.data, READ_STAGE);
}

void updateRequest(Viewer3DApp const& app)
{
    if (!app.data.needsUpdate())
        return;
    execute(app.data, INIT_TRACER);
    execute(app.data, TRACE_RAYS);
    execute(app.data, UPDATE_STEP);
}

void drawRequest(Viewer3DApp const& app)
{
    execute(app.data, DRAW_GENERATED_IMG);
}
