//
//  viewer3d.hpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#ifndef viewer3d_hpp
#define viewer3d_hpp

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "drawalgodata.hpp"
#include <iostream>

using namespace ci;
using namespace ci::app;
using namespace std;

class Viewer3DApp : public App {
public:
    mutable DrawAlgoData data;
    
    Viewer3DApp(): App() {
        data.step = 8;
        data.imgWidth = unsigned(getWindowWidth());
        data.imgHeight = unsigned(getWindowHeight());
        cout << "WxH: " << data.imgWidth << ' ' << data.imgHeight << '\n';
    }
    
    void setup() override;
    void update() override;
    void draw() override;
    void mouseDown(MouseEvent event) override;
    void keyDown(KeyEvent event) override;
    unsigned getImgWidth() const { return data.imgWidth; }
    unsigned getImgHeigtht() const { return data.imgHeight; }
    unsigned getStep() const { return data.step; }
};

#endif /* viewer3d_hpp */
