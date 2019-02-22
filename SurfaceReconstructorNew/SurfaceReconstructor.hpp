#pragma once

#include "ParticleData.hpp"
#include "ZIndexGrid.hpp"
#include "SPHHelper.hpp"
#include "SurfaceGrids.h"

#define OUTSIDE 999

class SurfaceReconstructor{
    private:
    ParticleData particleData;
    ZIndexGrid zGrid;
    SPHHelper sphHelper;
    SurfaceGrid surfaceGrid;
    
    float particleSpacing;
    float infectRadius;
    cfloat3 padding;
    float normThres;
    int neighborThres;
    veci surfaceParticleMark;

    public:
    SurfaceReconstructor(){
        particleSpacing = 1;
        infectRadius = 2;
        padding.Set(infectRadius,infectRadius,infectRadius);
        sphHelper.SetupCubic(infectRadius/2);
        normThres = 0.5;
        neighborThres = 25;
    }
    void LoadParticle(char* filePath){
        particleData.LoadFromFile(filePath);
        particleData.Analyze();
        surfaceParticleMark.resize(particleData.Size());
    }

    void SetupGrids(){
        SetupSurfaceGrid();
        SetupZGrid();
    }
    
    void SetupSurfaceGrid(){
        surfaceGrid.cellWidth = 0.5;  
        surfaceGrid.xmin = particleData.GetXMin()-padding;
        surfaceGrid.xmax = particleData.GetXMax()+padding;
        surfaceGrid.Init();
    }

    void SetupZGrid();
    
    void ExtractSurface();

    void SortParticles();
    void ReorderParticles();
    
    void ExtractSurfaceParticles();
    void ComputeColorFieldAndMarkParticles();

    void ExtractSurfaceVertices();

    void ComputeScalarValues();

    void Triangulate();

    void OutputMesh(char* filePath){

    }
};