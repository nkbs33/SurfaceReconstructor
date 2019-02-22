#pragma once

#include <utility>
#include "catpaw/vec_define.h"
#include "Grids.h"
#include "ParticleData.hpp"


struct ParticleHandler{
    int particleId;
    int particleHash;
};

class ZIndexGrid{
    
    public:
    float cellWidth;
    cfloat3 xmin;
    cfloat3 xmax;
    int resolution;
    int numParticles;
    int numCells;

    veci startIndices;
    veci endIndices;
    veci particleHashes;
    vector<ParticleHandler> particleHandlers;
    vector<Particle>* particles;
    vector<Particle> reorderBuffer;

    void SetXMin(cfloat3 x){
        xmin = x;
    }

    void SetXMax(cfloat3 x){
        xmax = x;
    }

    void SetCellWidth(float width){
        cellWidth = width;
    }

    void AdjustSize(){
        cfloat3 length = xmax - xmin;
        cint3 res = ceil(length / cellWidth);
        int maxres = fmax(fmax(res.x,res.y),res.z);
        maxres = ceil(log2f(maxres));
        maxres = pow(2, maxres);
        
        resolution = maxres;
        xmax = xmin + maxres * cellWidth;
        printf("grid resolution: %d\n", resolution);
        numCells = resolution*resolution*resolution;
        startIndices.resize(numCells);
        endIndices.resize(numCells);
    }

    void SortParticles(ParticleData& particleData){
        BindParticleData(particleData);
        
        ComputeParticleHash();
        SortParticleHash();
        ComputeStartIndices();
        ReorderParticleData();
    }

    void BindParticleData(ParticleData& particleData);

    void ComputeParticleHash();
    cint3 ComputeCoordinate(cfloat3& p);
    int ComputeCellHash(cint3& coord);

    void SortParticleHash();
    
    void ComputeStartIndices();

    void ReorderParticleData();

};