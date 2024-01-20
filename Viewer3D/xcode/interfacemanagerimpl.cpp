//
//  interfacemanagerimpl.cpp
//  Viewer3D
//
//  Created by Daniil on 24.11.2023.
//

#include "interfacemanagerimpl.hpp"
#include "operationmanager.hpp"
#include <map>

long n = 0;
map<unsigned, long> t;
map<unsigned, vector<long>> values;

void setupRequest(Viewer3DApp const& app)
{
    execute(app.data, READ_STAGE);
}

void updateRequest(Viewer3DApp const& app)
{
    clock_t t1 = clock();
    if (!app.data.needsUpdate()) {
        n++;
        if (n < 5)
            app.data.resumeUpdates();
        return;
    }
    execute(app.data, INIT_TRACER);
    execute(app.data, TRACE_RAYS);
    clock_t t2 = clock();
    t[app.data.step] += t2 - t1;
    values[app.data.step].push_back(t2 - t1);
    execute(app.data, UPDATE_STEP);
}

void drawRequest(Viewer3DApp const& app)
{
    ofstream out("/Users/danmac/3d-viewer/time");
    for (auto el: t)
        out << el.first << ' ' << double(el.second) / double(n) << endl;
    out.close();
    
    execute(app.data, DRAW_GENERATED_IMG);
}
