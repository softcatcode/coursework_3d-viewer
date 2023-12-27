//
//  logs.cpp
//  Viewer3D
//
//  Created by Daniil on 23.11.2023.
//

#include <iostream>
#include "logs.hpp"
using namespace std;

ostream& debug = cout;

void log(string const& message)
{
    cout << message << endl;
}
