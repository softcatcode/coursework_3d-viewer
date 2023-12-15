//
//  operationmanager.hpp
//  Viewer3D
//
//  Created by Daniil on 24.11.2023.
//

#ifndef operationmanager_hpp
#define operationmanager_hpp

#include "drawalgodata.hpp"

#define READ_STAGE 1
#define INIT_TRACER 2
#define TRACE_RAYS 3
#define DRAW_GENERATED_IMG 4
#define CLEAR_DRAW_DATA 5
#define UPDATE_STEP 6

void execute(DrawAlgoData& data, int request);

#endif /* operationmanager_hpp */
