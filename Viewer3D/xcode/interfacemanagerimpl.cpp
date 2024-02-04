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
    //execute(app.data, READ_STAGE);
    map<unsigned, float> values;
    execute(app.data, READ_STAGE);
    app.data.imgWidth = app.data.imgHeight = 600;
    while (app.data.imgWidth <= 800) {
        int n = 10;
        long sum = 0L;
        for (int i = 0; i < n; i++) {
            clock_t t1 = clock();
            execute(app.data, INIT_TRACER);
            execute(app.data, TRACE_RAYS);
            clock_t t2 = clock();
            sum += t2 - t1;
        }
        values[app.data.imgWidth] = double(sum) / double(n);
        cout << app.data.imgWidth << endl;
        app.data.imgWidth += 10;
        app.data.imgHeight += 10;
    }
    ofstream out("/Users/danmac/3d-viewer/time3");
    for (auto& elem: values) {
        out << elem.first << ' ' << elem.second << endl;
    }
    out.close();
    exit(0);
}

void updateRequest(Viewer3DApp const& app)
{
    if (!app.data.needsUpdate())
        return;
    if (app.data.needsTracerInit())
        execute(app.data, INIT_TRACER);
    execute(app.data, TRACE_RAYS);
    execute(app.data, UPDATE_STEP);
}

void drawRequest(Viewer3DApp const& app)
{
    execute(app.data, DRAW_GENERATED_IMG);
}
