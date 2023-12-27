//
//  controller.cpp
//  Viewer3D
//
//  Created by Daniil on 16.12.2023.
//

#include "controller.hpp"

void rotateUp(Stage& stage)
{
    // повернуть ось z наблюдателя вокруг оси x наблюдателя
    rotateCamera(stage.camera, 'x', -0.5f);
}

void rotateDown(Stage& stage)
{
    // повернуть ось z наблюдателя вокруг оси x наблюдателя
    rotateCamera(stage.camera, 'x', 0.5f);
}

void rotateRight(Stage& stage)
{
    // повернуть ось z наблюдателя вокруг оси y наблюдателя
    rotateCamera(stage.camera, 'y', 0.5f);
}

void rotateLeft(Stage& stage)
{
    // повернуть ось z наблюдателя вокруг оси y наблюдателя
    rotateCamera(stage.camera, 'y', -0.5f);
}

void moveForward(Stage& stage)
{
    translateCamera(stage.camera, vec3(0.f, 0.f, 100.f));
}

void moveBack(Stage& stage)
{
    translateCamera(stage.camera, vec3(0.f, 0.f, -100.f));
}

void moveRight(Stage& stage)
{
    translateCamera(stage.camera, vec3(100.f, 0.f, 0.f));
}

void moveLeft(Stage& stage)
{
    translateCamera(stage.camera, vec3(-100.f, 0.f, 0.f));
}

void increaseLight(Stage& stage)
{
    modifyLight(stage.sources, 100.f);
}

void decreaseLight(Stage& stage)
{
    modifyLight(stage.sources, -100.f);
}
