//
//  keyrespond.cpp
//  Viewer3D
//
//  Created by Daniil on 16.12.2023.
//

#include "keyrespond.hpp"
#include "controller.hpp"

using namespace app;
using namespace cinder;

void respondToKey(DrawAlgoData& data, int keyCode)
{
    switch (keyCode) {
        case KeyEvent::KEY_RIGHT:
            rotateRight(data.stage);
            break;
        case KeyEvent::KEY_LEFT:
            rotateLeft(data.stage);
            break;
        case KeyEvent::KEY_UP:
            rotateUp(data.stage);
            break;
        case KeyEvent::KEY_DOWN:
            rotateDown(data.stage);
            break;
        
        case KeyEvent::KEY_w:
            moveForward(data.stage);
            break;
        case KeyEvent::KEY_s:
            moveBack(data.stage);
            break;
        case KeyEvent::KEY_d:
            moveRight(data.stage);
            break;
        case KeyEvent::KEY_a:
            moveLeft(data.stage);
            break;
    }
    data.stopUpdate();
}
