//
//  mathutils.cpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#include "mathutils.hpp"
using namespace std;

Transformer buildTransformer(vec3 const& point, vec3 const& i, vec3 const& j, vec3 const& k)
{
    vec3 x(1.f, 0.f, 0.f), y(0.f, 1.f, 0.f), z(0.f, 0.f, 1.f);
    vec3 a(dot(x, i), dot(x, j), dot(x, k));
    vec3 b(dot(y, i), dot(y, j), dot(y, k));
    vec3 c(dot(z, i), dot(z, j), dot(z, k));
    mat3 matrix(vec3(a.x, b.x, c.x), vec3(a.y, b.y, c.y), vec3(a.z, b.z, c.z));
    vec3 bias = -1.f * (point * matrix);
    return { matrix, bias };
}

void transformPoint(vec3& point, Transformer const& transformer)
{
    point = transformer.bias + (point * transformer.matrix);
}

void transformVector(vec3& vector, Transformer const& transformer)
{
    vector = vector * transformer.matrix;
}

mat3 getPoints(Triangle const& tr, vector<vec3> const& points)
{
    return mat3(points[tr.a], points[tr.b], points[tr.c]);
}

void sortByY(vec3& a, vec3& b, vec3& c)
{
    if (a.y > b.y)
        swap(a, b);
    if (b.y > c.y)
        swap(b, c);
    if (a.y > b.y)
        swap(a, b);
}

Line2d buildLine(float x1, float y1, float x2, float y2)
{
    if (abs(x1 - x2) < EPS)
        return {0.f, 0.f, true, x1};
    float k = (y1 - y2) / (x1 - x2);
    float b = y1 - k * x1;
    return {k, b, false, 0.f};
}

int crossPixel(Line2d const& line, int i)
{
    if (line.vertical)
        return line.location;
    return (float(i) - line.b) / line.k;
}

vec3 reflectionDir(vec3 const& r, vec3 const& n)
{
    // length of vectors r and n is 1
    vec3 x = normalize(cross(n, cross(r, n)));
    return x * dot(x, r) - n * dot(n, r);
}

vec3 afterTransitionDir(vec3 const& r, vec3 const& n, float k1, float k2)
{
    vec3 x = normalize(cross(n, cross(r, n)));
    float sinA = dot(r, x); // (\vec{r};vec{R})
    float sinB = k1 / k2 * sinA;
    float cosB = sqrt(1 - sinB * sinB);
    return sinB * x - cosB * n;
}

Color colorAfterCollision(Color rayClor, Color surfaceColor, float power)
{
    return (rayClor + surfaceColor * power) / (power + 1.f);
}

float Triangle::depthOf(float x, float y) const
{
    return -(plane.a * x + plane.b * y + plane.d) / plane.c;
}

bool Triangle::perpendicularToCamera() const
{
    return abs(plane.c) < EPS;
}

void Triangle::setPlane(vec4 const& ratios)
{
    plane.a = ratios[0];
    plane.b = ratios[1];
    plane.c = ratios[2];
    plane.d = ratios[3];
}
