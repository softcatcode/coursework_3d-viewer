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

ifstream& operator >>(ifstream& in, vector<vec3>& vec);

ifstream& operator >>(ifstream& in, Triangle& t);

ifstream& operator >>(ifstream& in, vector<Triangle>& vec);

ifstream& operator >>(ifstream& in, Color& color);

ifstream& operator >>(ifstream& in, Object& obj);

ifstream& operator >>(ifstream& in, vector<Object>& vec);

ifstream& operator >>(ifstream& in, ObjectProperties& prop);

ifstream& operator >>(ifstream& in, LightSource& src);

ifstream& operator >>(ifstream& in, vector<LightSource>& vec);

ifstream& operator >>(ifstream& in, Observer& camera);

ifstream& operator >>(ifstream& in, Stage& stage);

void readStage(Stage& stage, string const& fileName);

#endif /* ioutils_hpp */
