//
//  keyrespond.cpp
//  Viewer3D
//
//  Created by Daniil on 16.12.2023.
//

#include "keyrespond.hpp"
#include "controller.hpp"
#include "logs.hpp"

using namespace app;
using namespace cinder;

void processRotations(DrawAlgoData& data, int keyCode)
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
    }
}

void processTranslations(DrawAlgoData& data, int keyCode)
{
    switch (keyCode) {
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
}

void processLightRegulation(DrawAlgoData& data, int keyCode)
{
    if (keyCode == KeyEvent::KEY_F2)
        increaseLight(data.stage);
    else if (keyCode == KeyEvent::KEY_F1)
        decreaseLight(data.stage);
}

void processShadowingRegulation(DrawAlgoData& data, int keyCode)
{
    switch (keyCode) {
        case KeyEvent::KEY_1:
            data.shadowingMethod = BrightnessCalcMethod::rsm;
            break;
        case KeyEvent::KEY_2:
            data.shadowingMethod = BrightnessCalcMethod::tracing;
            break;
        case KeyEvent::KEY_3:
            data.shadowingMethod = BrightnessCalcMethod::fong;
            break;
    }
}

void respondToKey(DrawAlgoData& data, int keyCode)
{
    processRotations(data, keyCode);
    processTranslations(data, keyCode);
    processLightRegulation(data, keyCode);
    processShadowingRegulation(data, keyCode);
    data.resumeUpdates();
}
