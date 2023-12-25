#include "viewer3d.hpp"
#include "keyrespond.hpp"
#include "logs.hpp"
#include "interfacemanager.hpp"
#include "cinder/app/RendererGl.h"
#include<ctime>
#include<iomanip>

void Viewer3DApp::setup()
{
    processRequest(*this, SETUP_REQUEST);
    log("setup");
}

void Viewer3DApp::update()
{
    unsigned initStep = data.step;
    clock_t t1 = clock();
    processRequest(*this, UPDATE_REQUEST);
    clock_t t2 = clock();
    if (initStep > 0)
        debug
            << "update " << data.shadowingMethod << ": "
            << fixed << setw(3) << setprecision(3)
            << double(t2 - t1) / 1e6 << ' ' << initStep << endl;
}

void Viewer3DApp::draw()
{
    processRequest(*this, DRAW_REQUEST);
}

void Viewer3DApp::mouseDown(MouseEvent event)
{
}

void Viewer3DApp::keyDown(KeyEvent event)
{
    respondToKey(data, event.getCode());
}

CINDER_APP(Viewer3DApp, RendererGl);
