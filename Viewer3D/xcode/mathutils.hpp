//
//  mathutils.hpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#ifndef mathutils_hpp
#define mathutils_hpp

#define EPS 1e-6f

#include <memory>

using namespace cinder;
using namespace std;

struct Transformer
{
    mat3 matrix;
    vec3 bias;
};

struct Line2d
{
    float k, b;
    bool vertical; // If the line is vertical then k and b are meaningless.
    float location; // x coordinate of a vertical line
};

struct PlaneEquation
{
    float a, b, c, d;
};

struct Triangle
{
    unsigned a, b, c;
    vec3 n;
    PlaneEquation plane;
    
    float depthOf(float x, float y) const;
    bool perpendicularToCamera() const;
    void setPlane(vec4 const& ratios);
};

Transformer buildTransformer(vec3 const& point, vec3 const& i, vec3 const& j, vec3 const& k);

void transformPoint(vec3& point, Transformer const& transformer);

void transformVector(vec3& vector, Transformer const& transformer);

mat3 getPoints(Triangle const& tr, vector<vec3> const& points);

void sortByY(vec3& a, vec3& b, vec3& c);

template <typename Type>
void resize(vector< vector<Type> >& vec, unsigned n, unsigned m)
{
    vec.resize(n);
    for (unsigned int i = 0; i < n; i++)
        vec[i].resize(m);
}

Line2d buildLine(float x1, float x2, float y1, float y2);

int crossPixel(Line2d const& line, int i);

vec3 reflectionDir(vec3 const& r, vec3 const& n);

vec3 afterTransitionDir(vec3 const& r, vec3 const& n, float k1, float k2);

Color colorAfterCollision(Color rayClor, Color surfaceColor, float power);

Color min(Color const& a, Color const& b);

#endif /* mathutils_hpp */
