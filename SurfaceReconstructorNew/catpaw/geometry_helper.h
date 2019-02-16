#pragma once
#include "geometry.h"

inline void printf3(const char* msg, cfloat3 f){
    printf("%s: %f %f %f\n",msg, f.x, f.y, f.z);
}