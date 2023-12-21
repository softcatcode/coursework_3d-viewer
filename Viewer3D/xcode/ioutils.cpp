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

ifstream& operator >>(ifstream& in, vector<vec3>& vec)
{
    for (auto& elem: vec)
        in >> elem;
    return in;
}

ifstream& operator >>(ifstream& in, Triangle& t)
{
    in >> t.a >> t.b >> t.c;
    in >> t.n;
    return in;
}

ifstream& operator >>(ifstream& in, vector<Triangle>& vec)
{
    for (auto& elem: vec)
        in >> elem;
    return in;
}

ifstream& operator >>(ifstream& in, Color& color)
{
    float r, g, b;
    in >> r >> g >> b;
    color = Color(r, g, b);
    return in;
}

ifstream& operator >>(ifstream& in, ObjectProperties& prop)
{
    in >> prop.color >> prop.transmission >> prop.reflection >> prop.diffuseRefl;
    return in;
}

ifstream& operator >>(ifstream& in, Object& obj)
{
    unsigned pointCount, triangleCount;
    in >> pointCount >> triangleCount;
    obj.points.resize(pointCount);
    obj.triangles.resize(triangleCount);
    in >> obj.points;
    in >> obj.triangles;
    in >> obj.properties;
    buildPlanesForTriangles(obj.triangles, obj.points);
    return in;
}

ifstream& operator >>(ifstream& in, vector<Object>& vec)
{
    for (auto& elem: vec)
        in >> elem;
    return in;
}

ifstream& operator >>(ifstream& in, LightSource& src)
{
    in >> src.location >> src.x >> src.y >> src.z;
    return in;
}

ifstream& operator >>(ifstream& in, vector<LightSource>& vec)
{
    for (auto& elem: vec)
        in >> elem;
    return in;
}

ifstream& operator >>(ifstream& in, Observer& cam)
{
    in >> cam.location >> cam.x >> cam.y >> cam.z;
    return in;
}

ifstream& operator >>(ifstream& in, Stage& stage)
{
    in >> stage.objects;
    in >> stage.sources;
    in >> stage.camera;
    return in;
}

Object readSphere()
{
    Object obj;
    int n;
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
    obj.properties = { cinder::Color(100, 200, 50), 0.2f, 0.5f, 0.3f, 1.5f };
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
    obj.properties = { cinder::Color(100, 200, 50), 0.3f, 0.4f, 0.3f, 1.5f };
    return obj;
}

void readStage(Stage& stage, string const& fileName)
{
    Object obj = createCube();
    stage.objects = { obj };
    mat4 trMatr = translate(vec3(0, 215, 20));
    for (auto& point: obj.points) {
        vec4 coords(point, 1);
        coords = trMatr * coords;
        point.x = coords.x;
        point.y = coords.y;
        point.z = coords.z;
    }
    buildPlanesForTriangles(obj.triangles, obj.points);
    stage.objects.push_back(obj);
    
    stage.camera = { vec3(220, -100, -100), vec3(0, 1, 0), vec3(0, 0, 1), vec3(-1, 0, 0) };
    stage.sources = { { vec3(215, 505, 205), vec3(-1, 0, 0), vec3(0, -1, 1), vec3(0, -1, -1), 800.f } };
    stage.sources[0].y = normalize(stage.sources[0].y);
    stage.sources[0].z = normalize(stage.sources[0].z);
}
