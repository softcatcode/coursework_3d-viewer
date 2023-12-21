#include "viewer3d.hpp"
#include "keyrespond.hpp"
#include "logs.hpp"
#include "interfacemanager.hpp"
#include "cinder/app/RendererGl.h"

void Viewer3DApp::setup()
{
    processRequest(*this, SETUP_REQUEST);
    log("setup");
}

void Viewer3DApp::update()
{
    processRequest(*this, UPDATE_REQUEST);
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
