//
//  drawutils.cpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#include "drawutils.hpp"
#include "drawalgodata.hpp"
#include "logs.hpp"
using namespace cinder;
using namespace gl;

void drawImage(Surface const& image)
{
    auto texture = Texture::create(image);
    draw(texture);
}

unsigned getNewDrawStep(unsigned step)
{
    return step > 1 ? step - 1 : step;
}

Surface enlarge(Surface const& img, unsigned k)
{
    unsigned width = img.getWidth() * k;
    unsigned height = img.getHeight() * k;
    Surface result(width, height, false);
    auto imgIter = img.getIter();
    auto resIter = result.getIter();
    
    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            unsigned y = i / k, x = j / k;
            resIter.r(j, i) = imgIter.r(x, y);
            resIter.g(j, i) = imgIter.g(x, y);
            resIter.b(j, i) = imgIter.b(x, y);
        }
    }
    return result;
}
