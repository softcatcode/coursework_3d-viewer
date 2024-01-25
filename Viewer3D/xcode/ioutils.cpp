//
//  ioutils.cpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#include "ioutils.hpp"

ifstream& operator >>(ifstream& in, vec3& v)
{
    in >> v.x >> v.y >> v.z;
    return in;
}

void print(ostream& out, vec3 const& v)
{
    out << v.x << ' ' << v.y << ' ' << v.z << '\n';
}

template <typename Type>
ifstream& operator >>(ifstream& in, vector<Type>& v)
{
    for (Type& elem: v)
        in >> elem;
    return in;
}

template <>
ostream& operator <<(ostream& out, vector<vec3> const& v)
{
    for (vec3 const& elem: v)
        print(out, elem);
    return out;
}

template <typename Type>
ostream& operator <<(ostream& out, vector<Type> const& v)
{
    for (auto const& elem: v)
        out << elem << '\n';
    return out;
}

ifstream& operator >>(ifstream& in, Triangle& t)
{
    in >> t.a >> t.b >> t.c;
    in >> t.n;
    return in;
}

ostream& operator <<(ostream& out, Triangle const& t)
{
    out << t.a << ' ' << t.b << ' ' << t.c << ' ';
    print(out, t.n);
    return out;
}

ifstream& operator >>(ifstream& in, Color& color)
{
    float r, g, b;
    in >> r >> g >> b;
    color = Color(r, g, b);
    return in;
}

void print(ostream& out, Color const& color)
{
    out << color.r << ' ' << color.g << ' ' << color.b << '\n';
}

ifstream& operator >>(ifstream& in, ObjectProperties& prop)
{
    in >> prop.color;
    in >> prop.transmission >> prop.reflection;
    in >> prop.optDensity;
    return in;
}

ostream& operator <<(ostream& out, ObjectProperties const& prop)
{
    print(out, prop.color);
    out <<
        prop.transmission << ' ' <<
        prop.reflection << ' ' <<
        prop.optDensity << '\n';
    return out;
}

ifstream& operator >>(ifstream& in, Object& obj)
{
    unsigned pointCount;
    in >> pointCount;
    obj.points.resize(pointCount);
    in >> obj.points;
    
    unsigned triangleCount;
    in >> triangleCount;
    obj.triangles.resize(triangleCount);
    in >> obj.triangles;
    
    buildPlanesForTriangles(obj.triangles, obj.points);
    in >> obj.properties;
    return in;
}

ostream& operator <<(ostream& out, Object const& obj)
{
    out << obj.points.size() << '\n';
    out << obj.points;
    out << obj.triangles.size() << '\n';
    out << obj.triangles;
    out << obj.properties << '\n';
    return out;
}

ifstream& operator >>(ifstream& in, LightSource& src)
{
    in >> src.location >> src.x >> src.y >> src.z >> src.color;
    return in;
}

ostream& operator <<(ostream& out, LightSource const& src)
{
    print(out, src.location);
    print(out, src.x);
    print(out, src.y);
    print(out, src.z);
    print(out, src.color);
    return out;
}

ifstream& operator >>(ifstream& in, Observer& cam)
{
    in >> cam.location >> cam.x >> cam.y >> cam.z;
    return in;
}

ostream& operator <<(ostream& out, Observer const& cam)
{
    print(out, cam.location);
    print(out, cam.x);
    print(out, cam.y);
    print(out, cam.z);
    return out;
}

ifstream& operator >>(ifstream& in, Stage& stage)
{
    unsigned objCount;
    in >> objCount;
    stage.objects.resize(objCount);
    in >> stage.objects;
    
    unsigned srcCount;
    in >> srcCount;
    stage.sources.resize(srcCount);
    in >> stage.sources;
    
    in >> stage.camera;
    return in;
}

ostream& operator <<(ostream& out, Stage const& stage)
{
    out << stage.objects.size() << '\n';
    out << stage.objects;
    out << stage.sources.size() << '\n';
    out << stage.sources;
    out << stage.camera << '\n';
    return out;
}

Object readSphere()
{
    Object obj;
    unsigned n;
    string path = "/Users/danmac/3d-viewer/Sphere/";
    
    ifstream pointInput(path + "POINTS.txt");
    pointInput >> n;
    obj.points.resize(n);
    pointInput >> obj.points;
    pointInput.close();
    
    ifstream triangleInput(path + "POLYGONS.txt");
    ifstream normInput(path + "NORM.txt");
    triangleInput >> n;
    for (int i = 0; i < n; i++) {
        Triangle t;
        triangleInput >> t.a >> t.b >> t.c;
        normInput >> t.n;
        t.n = normalize(t.n);
        obj.triangles.push_back(t);
    }
    normInput.close();
    triangleInput.close();
    buildPlanesForTriangles(obj.triangles, obj.points);
    obj.properties = { cinder::Color(100, 200, 50), 0.5f, 0.5f, 1.5f };
    return obj;
}

Object createCube()
{
    vector<vec3> points = {
        vec3(105, 0, 0),
        vec3(210, 105, 0),
        vec3(105, 210, 0),
        vec3(0, 105, 0),
        vec3(105, 0, 180),
        vec3(210, 105, 180),
        vec3(105, 210, 180),
        vec3(0, 105, 180)
    };
    PlaneEquation p;
    vec3 n1 = normalize(vec3(-1, -1, 0));
    vec3 n2 = normalize(vec3(1, -1, 0));
    vec3 n3 = normalize(vec3(1, 1, 0));
    vec3 n4 = normalize(vec3(-1, 1, 0));
    vec3 n5 = normalize(vec3(0, 0, -1));
    vec3 n6 = normalize(vec3(0, 0, 1));
    vector<Triangle> triangles = {
        {1, 4, 5, n2, p},
        {0, 1, 4, n2, p},
        {1, 5, 6, n3, p},
        {1, 6, 2, n3, p},
        {2, 7, 6, n4, p},
        {2, 3, 7, n4, p},
        {0, 3, 7, n1, p},
        {0, 4, 7, n1, p},
        {0, 1, 3, n5, p},
        {1, 2, 3, n5, p},
        {4, 5, 7, n6, p},
        {5, 6, 7, n6, p}
    };
    buildPlanesForTriangles(triangles, points);
    Object obj;
    obj.triangles = triangles;
    obj.points = points;
    obj.properties = { cinder::Color(100, 200, 50), 0.3f, 0.4f, 1.5f };
    return obj;
}

void readStage(Stage& stage, string const& fileName)
{
    ifstream in("/Users/danmac/3d-viewer/stages/" + fileName);
    in >> stage;
    in.close();
}
