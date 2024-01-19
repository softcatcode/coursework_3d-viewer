//
//  ioutils.hpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#ifndef ioutils_hpp
#define ioutils_hpp

#include "mathutils.hpp"
#include "stage.hpp"
#include <fstream>
using namespace std;
using namespace cinder;

ifstream& operator >>(ifstream& in, vec3& v);

void print(ostream& out, vec3 const& v);

template <typename Type>
ifstream& operator >>(ifstream& in, vector<Type>& v);

template <typename Type>
ostream& operator <<(ostream& out, vector<Type> const& v);

template <>
ostream& operator <<(ostream& out, vector<vec3> const& v);

ifstream& operator >>(ifstream& in, Triangle& t);

ostream& operator <<(ostream& out, Triangle const& t);

ifstream& operator >>(ifstream& in, Color& color);

void print(ostream& out, Color const& color);

ifstream& operator >>(ifstream& in, Object& obj);

ostream& operator <<(ostream& out, Object const& obj);

ifstream& operator >>(ifstream& in, ObjectProperties& prop);

ostream& operator <<(ostream& out, ObjectProperties const& prop);

ifstream& operator >>(ifstream& in, LightSource& src);

ostream& operator <<(ostream& out, LightSource const& src);

ifstream& operator >>(ifstream& in, Observer& cam);

ostream& operator <<(ostream& out, Observer const& cam);

ifstream& operator >>(ifstream& in, Stage& stage);

ostream& operator <<(ostream& out, Stage const& stage);

void readStage(Stage& stage, string const& fileName);

#endif /* ioutils_hpp */
