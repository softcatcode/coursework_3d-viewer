//
//  interfacemanagerimpl.cpp
//  Viewer3D
//
//  Created by Daniil on 24.11.2023.
//

#include "interfacemanagerimpl.hpp"
#include "operationmanager.hpp"

bool update = true;

void setupRequest(Viewer3DApp const& app)
{
    execute(app.data, READ_STAGE);
    execute(app.data, INIT_TRACER);
}

void updateRequest(Viewer3DApp const& app)
{
    if (!update)
        return;
    execute(app.data, TRACE_RAYS);
    execute(app.data, UPDATE_STEP);
    update = false;
}

void drawRequest(Viewer3DApp const& app)
{
    execute(app.data, DRAW_GENERATED_IMG);
}
