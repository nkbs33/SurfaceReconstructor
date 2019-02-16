#pragma once
#include "catpaw/geometry.h"

class SPHHelper{
    private:
    float smoothRadius;
    float factor;
    float gradientFactor;
    float laplacianFactor;

    public:
    void SetupCubic(float smoothRadius_){
        smoothRadius = smoothRadius_;
        factor = 1/3.141593/pow(smoothRadius,3);
        gradientFactor = 1.5/3.141593/pow(smoothRadius,4);
    }
    float Cubic(float d){
        float q = d/smoothRadius;
        float func;
        if(q>=2) 
            return 0;
        if(q>=1) 
            func = (2-q)*(2-q)*(2-q)*0.25;
        else
            func = (0.666667 - q*q + 0.5*q*q*q)*1.5;
        return func * factor;
    }
    cfloat3 CubicGradient(cfloat3 xij){
        float d = xij.Norm();
        float q = d/smoothRadius;
        if(!(q<2 && q>EPSILON))
            return cfloat3(0,0,0);
        float func;
        if(q>=1)
            func = 0.5*(2-q)*(2-q)*(-1);
        else
            func = (-2*q + 1.5*q*q);
        return xij * func * gradientFactor / d;
    }
};
